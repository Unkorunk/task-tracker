#include "IssueWidget.h"
#include "ui_IssueWidget.h"
#include "MainWindow.h"
#include "LabelPropertyWidget.h"
#include "DateTimePropertyWidget.h"
#include "UserSelectorWidget.h"
#include "IntegerSelectorWidget.h"
#include "CommentWidgetItem.h"
#include <QScrollBar>
#include "ValueSelectorWidget.h"

// TODO: refactor task_info
IssueWidget::IssueWidget(QWidget *parent) : AbstractPage(parent),
        ui(new Ui::IssueWidget) {

    ui->setupUi(this);

    SetupPage();

    connect(ui->editButton, &QPushButton::clicked, this, &IssueWidget::OnEditClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &IssueWidget::OnSaveClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &IssueWidget::OnDeleteClicked);

    connect(&Backend::Instance, &Backend::TaskEdited, this, &IssueWidget::OnTaskUpdated);
    connect(&Backend::Instance, &Backend::TaskDeleted, this, &IssueWidget::OnTaskDeleted);

    connect(&Backend::Instance, &Backend::CommentCreated, this, &IssueWidget::OnCommentCreated);
    connect(&Backend::Instance, &Backend::CommentDeleted, this, &IssueWidget::OnCommentDeleted);
    connect(&Backend::Instance, &Backend::CommentsLoaded, this, &IssueWidget::OnCommentsLoaded);

    ui->commentsList->verticalScrollBar()->setSingleStep(4);
    connect(&Backend::Instance, &Backend::TagCaptionsLoaded, this, &IssueWidget::OnTagsLoaded);
    connect(&Backend::Instance, &Backend::ProjectUsersLoaded, this, &IssueWidget::OnTeamLoaded);

    connect(&Backend::Instance, &Backend::TagAdded, this, &IssueWidget::OnTagAdded);
    connect(&Backend::Instance, &Backend::TagRemoved, this, &IssueWidget::OnTagRemoved);
}

IssueWidget::~IssueWidget() {
    delete ui;
}

void IssueWidget::SetupPage() {
    ui->editButton->hide();
    ui->deleteButton->hide();
    TaskInfo task = myContext.GetTask();

    if (task.GetId() != -1) {
        Backend::Instance.GetTagCaptions(myContext.GetCurrentProject());
        Backend::Instance.GetProjectUsers(myContext.GetCurrentProject());
    }

    this->ui->taskNameEdit->setText(task.GetTitle());
    this->ui->descriptionEdit->setText(task.GetDescription());
    this->ui->propertyList->clear();

    myCreatorSelector = new UserSelectorWidget(this);
    myCreatorSelector->SetData("Creator", task.GetCreator(), QList<UserInfo> { myContext.GetUser() });
    AddPropertyItem(myCreatorSelector);

    auto creationTimeWidget = new DateTimePropertyWidget(this);
    creationTimeWidget->SetData("Creation time", task.GetCreationTime());
    AddPropertyItem(creationTimeWidget);


    myUpdaterSelector = new UserSelectorWidget(this);
    myUpdaterSelector->SetData("Updater", task.GetUpdater(), QList<UserInfo> { myContext.GetUser() });
    AddPropertyItem(myUpdaterSelector);

    myUpdateTimeSelector = new DateTimePropertyWidget(this);
    myUpdateTimeSelector->SetData("Update time", task.GetUpdateTime());
    AddPropertyItem(myUpdateTimeSelector);

    myDeadlineSelector = new DateTimePropertyWidget(this);
    myDeadlineSelector->SetData("Deadline", task.GetDeadline());
    myDeadlineSelector->SetEditable(true);
    AddPropertyItem(myDeadlineSelector);

    myAssigneeSelector = new UserSelectorWidget(this);
    myAssigneeSelector->SetData("Assignee", task.GetAssignee(), QList<UserInfo> { myContext.GetUser() });
    myAssigneeSelector->SetEditable(true);
    AddPropertyItem(myAssigneeSelector);

    myStorypointSelector = new IntegerSelectorWidget(this);
    myStorypointSelector->SetData("Story points", task.GetStoryPoints());
    myStorypointSelector->SetEditable(true);
    AddPropertyItem(myStorypointSelector);

    UnlockUi();
    ToReadOnlyMode();

    UpdateComments();

    RoleInfo role = myContext.GetCurrentRole();
    canEdit = false;
    if (task.GetCreator().has_value() && task.GetCreator().value().GetId() == myContext.GetUser().GetId()) {
        if (role.HasPermission(RoleInfo::MANAGE_OWN_TASK)) {
            canEdit = true;
        }
    } else if (role.HasPermission(RoleInfo::MANAGE_ALL_TASK)) {
        canEdit = true;
    }

    if (canEdit) {
        ui->editButton->show();
        ui->deleteButton->show();
    }
}

void IssueWidget::OnEditClicked() {
    if (this->edited){
        // Cancel btn
        ToReadOnlyMode();

        TaskInfo task = myContext.GetTask();

        this->ui->taskNameEdit->setText(task.GetTitle());
        this->ui->descriptionEdit->setText(task.GetDescription());

        myAssigneeSelector->ChangeData(task.GetAssignee());
        myDeadlineSelector->ChangeData(task.GetDeadline());
        myStorypointSelector->ChangeData(task.GetStoryPoints());
    } else {
        // Edit btn
        ToEditMode();
    }
}

void IssueWidget::OnSaveClicked() {
    if (ui->taskNameEdit->text().count() == 0) {
        return;
    }

    LockUi();
    TaskInfo oldTask = myContext.GetTask();
    TaskInfo taskInfo(oldTask.GetId(), oldTask.GetProject(), ui->taskNameEdit->text(), ui->descriptionEdit->toPlainText(),
                      oldTask.GetCreator(), oldTask.GetCreationTime(), oldTask.GetStoryPoints());

    taskInfo.SetDeadline(myDeadlineSelector->GetData());
    taskInfo.SetAssignee(myAssigneeSelector->GetData());
    taskInfo.SetStoryPoints(myStorypointSelector->GetData());
    // TODO: is current time?
    taskInfo.SetUpdater(myContext.GetUser(), QDateTime::currentDateTime());
    Backend::Instance.EditTask(taskInfo);
}

void IssueWidget::OnDeleteClicked() {
    LockUi();
    Backend::Instance.DeleteTask(myContext.GetTask());
}

void IssueWidget::OnTaskUpdated(Status status, const TaskInfo& task) {
    UnlockUi();
    if (status.isSuccess) {
//        TaskInfo task = myContext.GetTask();

//        //save
//        task.SetDescription(this->ui->descriptionEdit->toPlainText());
//        task.SetTitle(this->ui->taskNameEdit->text());
//        task.SetDeadline(myDeadlineSelector->GetData());
//        task.SetStoryPoints(myStorypointSelector->GetData());
//        task.SetAssignee(myAssigneeSelector->GetData());
//        task.SetUpdater(myContext.GetUser(), QDateTime::currentDateTimeUtc());

        myContext.SetTask(task);
        SetupPage();

        //and lock
        ToReadOnlyMode();
    } else {
        emit Backend::Instance.RequestFailed("Не удалось обновить задачу!");
    }
}

void IssueWidget::OnTaskDeleted(Status status) {
    UnlockUi();
    if (status.isSuccess) {
        emit TaskDeleted(MainWindow::Transition::Project, myContext);
    }
}

void IssueWidget::OnCommentCreated(Status status, const CommentInfo &comment) {
    if (status.isSuccess) {
         Backend::Instance.GetComments(myContext.GetTask());
    }
}

void IssueWidget::OnCommentDeleted(Status status) {
    if (status.isSuccess) {
         Backend::Instance.GetComments(myContext.GetTask());
    }
}

void IssueWidget::OnCommentsLoaded(Status status, const QList<CommentInfo> &comments) {
    if (status.isSuccess) {
        TaskInfo task = myContext.GetTask();
        task.SetComments(comments);
        myContext.SetTask(task);
        UpdateComments();
    }
}

void IssueWidget::OnTagsLoaded(Status status, const QList<TagInfo> &tags) {
    if (!status.isSuccess) {
        emit Backend::Instance.RequestFailed("Не удалось получить информацию о тегах!");
        return;
    }

    myContext.SetTags(tags);
    UpdateTags();
}

void IssueWidget::OnTeamLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>> &team) {
    if (!status.isSuccess) {
        emit Backend::Instance.RequestFailed("Не удалось получить информацию о команде!");
        return;
    }

    myContext.SetProjectTeam(team);
    QList<UserInfo> users;
    for (auto& it : team) {
        users.push_back(it.first);
    }

    myCreatorSelector->SetUsers(users);
    myUpdaterSelector->SetUsers(users);
    myAssigneeSelector->SetUsers(users);
}

void IssueWidget::OnTagAdded(Status status, const TaskTag &taskTag) {
    if (!status.isSuccess) {
        emit Backend::Instance.RequestFailed("Не удалось добавить тег!");
        return;
    }

    Backend::Instance.EditTask(myContext.GetTask());
}

void IssueWidget::OnTagRemoved(Status status) {
    if (!status.isSuccess) {
        emit Backend::Instance.RequestFailed("Не удалось удалить тег!");
        return;
    }

    Backend::Instance.EditTask(myContext.GetTask());
}

void IssueWidget::UpdateComments() {
    TaskInfo task = myContext.GetTask();
    ui->commentsList->clear();

    RoleInfo role = myContext.GetCurrentRole();

    if (role.HasPermission(RoleInfo::MANAGE_OWN_COMMENT)) {
        auto item = new QListWidgetItem();
        auto widget = new CommentWidgetItem(this);
        widget->SetupNewCommentMode(task);
        item->setSizeHint(QSize(200, 150));
        ui->commentsList->addItem(item);
        ui->commentsList->setItemWidget(item, widget);
    }

    for (const CommentInfo& comment : task.GetComments()) {
        auto item = new QListWidgetItem();
        auto widget = new CommentWidgetItem(this);
        widget->SetupComment(comment, myContext);
        item->setSizeHint(QSize(200, 150));

        ui->commentsList->addItem(item);
        ui->commentsList->setItemWidget(item, widget);
    }
}

void IssueWidget::UpdateTags() {
    TaskInfo task = myContext.GetTask();
    QList<TagInfo> tags = myContext.GetTags();

    for (auto& taskTag : task.GetTags()) {
        ValueSelectorWidget* valueSelector = new ValueSelectorWidget(this);
        valueSelector->SetupTags(tags, task);
        valueSelector->SetupTag(taskTag, canEdit);
        AddPropertyItem(valueSelector);
    }

    if (canEdit) {
        ValueSelectorWidget* valueSelector = new ValueSelectorWidget(this);
        valueSelector->SetupTags(tags, task);
        AddPropertyItem(valueSelector);
    }
}

void IssueWidget::ToEditMode() {
    this->edited = true;
    this->ui->editButton->setText("Cancel");
    this->ui->saveButton->setVisible(true);

    this->ui->taskNameEdit->setEnabled(true);
    this->ui->taskNameEdit->setReadOnly(false);
    this->ui->descriptionEdit->setReadOnly(false);

    myDeadlineSelector->SetEditable(true);
    myAssigneeSelector->SetEditable(true);
    myStorypointSelector->SetEditable(true);

    this->ui->descriptionEdit->setStyleSheet("background-color: white;");
    this->ui->taskNameEdit->setStyleSheet("background-color: white; border: 1px solid black");
}

void IssueWidget::ToReadOnlyMode() {
    this->edited = false;
    this->ui->editButton->setText("Edit");
    this->ui->saveButton->setVisible(false);

    this->ui->taskNameEdit->setEnabled(false);
    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);

    this->ui->descriptionEdit->setStyleSheet("background-color: rgb(232, 227, 227);");
    this->ui->taskNameEdit->setStyleSheet("background-color: rgb(232, 227, 227); border: none");

    myDeadlineSelector->SetEditable(false);
    myAssigneeSelector->SetEditable(false);
    myStorypointSelector->SetEditable(false);
}

void IssueWidget::LockUi() {
    this->ui->editButton->setEnabled(false);
    this->ui->saveButton->setEnabled(false);
    this->ui->deleteButton->setEnabled(false);

    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);

    myDeadlineSelector->SetEditable(false);
    myAssigneeSelector->SetEditable(false);
    myStorypointSelector->SetEditable(false);
}

void IssueWidget::UnlockUi() {
    this->ui->editButton->setEnabled(true);
    this->ui->saveButton->setEnabled(true);
    this->ui->deleteButton->setEnabled(true);

    this->ui->taskNameEdit->setReadOnly(false);
    this->ui->descriptionEdit->setReadOnly(false);

    myDeadlineSelector->SetEditable(true);
    myAssigneeSelector->SetEditable(true);
    myStorypointSelector->SetEditable(true);
}

void IssueWidget::AddPropertyItem(QWidget *propertyWidget) {
    auto item = new QListWidgetItem();
    item->setSizeHint(QSize(200, 50));

    ui->propertyList->addItem(item);
    ui->propertyList->setItemWidget(item, propertyWidget);
}
