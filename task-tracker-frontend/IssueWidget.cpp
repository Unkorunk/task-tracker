#include "IssueWidget.h"
#include "ui_IssueWidget.h"
#include "MainWindow.h"
#include "LabelPropertyWidget.h"
#include "DateTimePropertyWidget.h"
#include "UserSelectorWidget.h"
#include "IntegerSelectorWidget.h"

IssueWidget::IssueWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IssueWidget),
    task_info(-1, ProjectInfo(-1, ""), "", "", std::optional<UserInfo>(), QDateTime::currentDateTime(), 0),
    project_info(-1, "")
{
    this->ui->setupUi(this);
    this->SetupTask(project_info, task_info);

    connect(ui->editButton, &QPushButton::clicked, this, &IssueWidget::OnEditClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &IssueWidget::OnSaveClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &IssueWidget::OnDeleteClicked);
    connect(&Backend::Instance, &Backend::TaskEdited, this, &IssueWidget::OnTaskUpdated);
    connect(&Backend::Instance, &Backend::TaskDeleted, this, &IssueWidget::OnTaskDeleted);
}

IssueWidget::~IssueWidget()
{
    delete ui;
}

void IssueWidget::OnEditClicked(){
    if (this->edited){
        // Cancel btn
        ToReadOnlyMode();
        this->ui->taskNameEdit->setText(task_info.GetTitle());
        this->ui->descriptionEdit->setText(task_info.GetDescription());
        myAssigneeSelector->ChangeData(task_info.GetAssignee());
        myDeadlineSelector->ChangeData(task_info.GetDeadline());
        myStorypointSelector->ChangeData(task_info.GetStoryPoints());
    } else {
        // Edit btn
        ToEditMode();
    }
}

void IssueWidget::OnSaveClicked(){
    if (ui->taskNameEdit->text().count() == 0) {
        return;
    }

    LockUi();
    MainWindow::Instance->StartLoading();
    TaskInfo taskInfo(task_info.GetId(), task_info.GetProject(), ui->taskNameEdit->text(),
                      ui->descriptionEdit->toPlainText(), task_info.GetCreator(), task_info.GetCreationTime(), task_info.GetStoryPoints());

    taskInfo.SetDeadline(myDeadlineSelector->GetData());
    taskInfo.SetAssignee(myAssigneeSelector->GetData());
    taskInfo.SetStoryPoints(myStorypointSelector->GetData());
    // TODO: is current time?
    taskInfo.SetUpdater(Backend::Instance.GetProfile(), QDateTime::currentDateTime());
    Backend::Instance.EditTask(taskInfo);
}

void IssueWidget::OnDeleteClicked()
{
    LockUi();
    MainWindow::Instance->StartLoading();
    Backend::Instance.DeleteTask(task_info);
}

void IssueWidget::OnTaskUpdated(Status status, const TaskInfo& task)
{
    UnlockUi();
    MainWindow::Instance->StopLoading();
    if (status.isSuccess) {
        //save
        task_info.SetDescription(this->ui->descriptionEdit->toPlainText());
        task_info.SetTitle(this->ui->taskNameEdit->text());
        task_info.SetDeadline(myDeadlineSelector->GetData());
        task_info.SetStoryPoints(myStorypointSelector->GetData());
        task_info.SetAssignee(myAssigneeSelector->GetData());
        task_info.SetUpdater(Backend::Instance.GetProfile(), QDateTime::currentDateTimeUtc());

        //and lock
        ToReadOnlyMode();
    } else {
        // TODO: handle errors;
    }
}

void IssueWidget::OnTaskDeleted(Status status)
{
    UnlockUi();
    MainWindow::Instance->StopLoading();
    if (status.isSuccess) {
        MainWindow::Instance->OnProjectTransition(project_info);
    }
}

// TODO: refactor, add users
void IssueWidget::SetupTask(const ProjectInfo& project, const TaskInfo &task)
{
    project_info = project;
    InternalSetupTask(task);
}

void IssueWidget::ToEditMode()
{
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

void IssueWidget::ToReadOnlyMode()
{
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

void IssueWidget::InternalSetupTask(const TaskInfo &task)
{
    task_info = task;

    this->ui->taskNameEdit->setText(task.GetTitle());
    this->ui->descriptionEdit->setText(task.GetDescription());

    this->ui->propertyList->clear();

    auto creatorWidget = new UserSelectorWidget(this);
    creatorWidget->SetData("Creator", task.GetCreator(), QList<UserInfo> { Backend::Instance.GetProfile() });
    AddPropertyItem(creatorWidget);

    auto creationTimeWidget = new DateTimePropertyWidget(this);
    creationTimeWidget->SetData("Creation time", task.GetCreationTime());
    AddPropertyItem(creationTimeWidget);


    myUpdaterSelector = new UserSelectorWidget(this);
    myUpdaterSelector->SetData("Updater", task.GetUpdater(), QList<UserInfo> { Backend::Instance.GetProfile() });
    AddPropertyItem(myUpdaterSelector);

    myUpdateTimeSelector = new DateTimePropertyWidget(this);
    myUpdateTimeSelector->SetData("Update time", task.GetUpdateTime());
    AddPropertyItem(myUpdateTimeSelector);

    myDeadlineSelector = new DateTimePropertyWidget(this);
    myDeadlineSelector->SetData("Deadline", task.GetDeadline());
    myDeadlineSelector->SetEditable(true);
    AddPropertyItem(myDeadlineSelector);

    myAssigneeSelector = new UserSelectorWidget(this);
    myAssigneeSelector->SetData("Assignee", task.GetAssignee(), QList<UserInfo> {Backend::Instance.GetProfile() });
    myAssigneeSelector->SetEditable(true);
    AddPropertyItem(myAssigneeSelector);

    myStorypointSelector = new IntegerSelectorWidget(this);
    myStorypointSelector->SetData("Story points", task.GetStoryPoints());
    myStorypointSelector->SetEditable(true);
    AddPropertyItem(myStorypointSelector);

    UnlockUi();
    ToReadOnlyMode();
}

void IssueWidget::LockUi()
{
    this->ui->editButton->setEnabled(false);
    this->ui->saveButton->setEnabled(false);
    this->ui->deleteButton->setEnabled(false);

    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);

    myDeadlineSelector->SetEditable(false);
    myAssigneeSelector->SetEditable(false);
    myStorypointSelector->SetEditable(false);
}

void IssueWidget::UnlockUi()
{
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
