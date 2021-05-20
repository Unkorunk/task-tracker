#include "IssueWidget.h"
#include "ui_IssueWidget.h"
#include "MainWindow.h"

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

void IssueWidget::OnTaskUpdated(Status status)
{
    UnlockUi();
    MainWindow::Instance->StopLoading();
    if (status.isSuccess) {
        //save
        task_info.SetDescription(this->ui->descriptionEdit->toPlainText());
        task_info.SetTitle(this->ui->taskNameEdit->text());
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

void IssueWidget::SetupTask(const ProjectInfo& project, const TaskInfo &task)
{
    project_info = project;
    task_info = task;

    this->ui->taskNameEdit->setText(task.GetTitle());
    this->ui->descriptionEdit->setText(task.GetDescription());

    this->ui->statusComboBox->clear();
    this->ui->statusComboBox->addItem("TODO");
    this->ui->statusComboBox->addItem("In progress");
    this->ui->statusComboBox->addItem("Done");

    UnlockUi();
    ToReadOnlyMode();
}

void IssueWidget::ToEditMode()
{
    this->edited = true;
    this->ui->editButton->setText("Cancel");
    this->ui->saveButton->setVisible(true);

    this->ui->taskNameEdit->setStyleSheet("* { background-color: rgba(255, 255, 255, 255); }");

    this->ui->taskNameEdit->setReadOnly(false);
    this->ui->descriptionEdit->setReadOnly(false);
}

void IssueWidget::ToReadOnlyMode()
{
    this->edited = false;
    this->ui->editButton->setText("Edit");
    this->ui->saveButton->setVisible(false);

    this->ui->taskNameEdit->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");

    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);
}

void IssueWidget::LockUi()
{
    this->ui->editButton->setEnabled(false);
    this->ui->saveButton->setEnabled(false);
    this->ui->deleteButton->setEnabled(false);

    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);
}

void IssueWidget::UnlockUi()
{
    this->ui->editButton->setEnabled(true);
    this->ui->saveButton->setEnabled(true);
    this->ui->deleteButton->setEnabled(true);

    this->ui->taskNameEdit->setReadOnly(false);
    this->ui->descriptionEdit->setReadOnly(false);
}
