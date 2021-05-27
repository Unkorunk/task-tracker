#include "IssueWidget.h"
#include "ui_IssueWidget.h"
#include "MainWindow.h"
#include "LabelPropertyWidget.h"
#include "DateTimePropertyWidget.h"
#include "UserSelectorWidget.h"
#include "IntegerSelectorWidget.h"

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
}

IssueWidget::~IssueWidget() {
    delete ui;
}

void IssueWidget::SetupPage() {
    TaskInfo task = myContext.GetTask();

    ui->statusComboBox->clear();
    ui->statusComboBox->addItem("Not started");
    ui->statusComboBox->addItem("TODO");
    ui->statusComboBox->addItem("Completed");

    this->ui->taskNameEdit->setText(task.GetTitle());
    this->ui->descriptionEdit->setText(task.GetDescription());

    this->ui->propertyList->clear();

    auto creatorWidget = new UserSelectorWidget(this);
    creatorWidget->SetData("Creator", task.GetCreator(), QList<UserInfo> { myContext.GetUser() });
    AddPropertyItem(creatorWidget);

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
        TaskInfo task = myContext.GetTask();

        //save
        task.SetDescription(this->ui->descriptionEdit->toPlainText());
        task.SetTitle(this->ui->taskNameEdit->text());
        task.SetDeadline(myDeadlineSelector->GetData());
        task.SetStoryPoints(myStorypointSelector->GetData());
        task.SetAssignee(myAssigneeSelector->GetData());
        task.SetUpdater(myContext.GetUser(), QDateTime::currentDateTimeUtc());

        myContext.SetTask(task);

        //and lock
        ToReadOnlyMode();
    } else {
        // TODO: handle errors;
    }
}

void IssueWidget::OnTaskDeleted(Status status) {
    UnlockUi();
    if (status.isSuccess) {
        emit TaskDeleted(MainWindow::Transition::Project, myContext);
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
}

void IssueWidget::ToReadOnlyMode() {
    this->edited = false;
    this->ui->editButton->setText("Edit");
    this->ui->saveButton->setVisible(false);

    this->ui->taskNameEdit->setEnabled(false);
    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);

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
