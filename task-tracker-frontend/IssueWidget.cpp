#include "IssueWidget.h"
#include "ui_IssueWidget.h"

IssueWidget::IssueWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IssueWidget),
    task_info(0, 0, "Task name", "Task description")
{
    this->ui->setupUi(this);
    this->SetupTask(task_info);

    connect(ui->editButton, &QPushButton::clicked, this, &IssueWidget::OnEditClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &IssueWidget::OnSaveClicked);
    connect(&Backend::Instance, &Backend::TaskEdited, this, &IssueWidget::OnTaskUpdated);
}

IssueWidget::~IssueWidget()
{
    delete ui;
}

void IssueWidget::OnEditClicked(){
    if (this->edited){
        // Cancel btn
        ToReadOnlyMode();
        this->ui->taskNameEdit->setText(task_info.taskName);
        this->ui->descriptionEdit->setText(task_info.taskDescription);
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
    Backend::Instance.EditTask(TaskInfo(task_info.taskId, task_info.projectId, ui->taskNameEdit->text(), ui->descriptionEdit->toPlainText()));
}

void IssueWidget::OnTaskUpdated(Status status)
{
    UnlockUi();
    if (status.isSuccess) {
        //save
        task_info.taskDescription = this->ui->descriptionEdit->toPlainText();
        task_info.taskName = this->ui->taskNameEdit->text();
        //and lock
        ToReadOnlyMode();
    } else {
        // TODO: handle errors;
    }
}

void IssueWidget::SetupTask(const TaskInfo &task)
{
    task_info = task;

    this->ui->taskNameEdit->setText(task.taskName);
    this->ui->descriptionEdit->setText(task.taskDescription);

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
    this->ui->taskNameEdit->setReadOnly(true);
    this->ui->descriptionEdit->setReadOnly(true);
}

void IssueWidget::UnlockUi()
{
    this->ui->editButton->setEnabled(true);
    this->ui->saveButton->setEnabled(true);
    this->ui->taskNameEdit->setReadOnly(false);
    this->ui->descriptionEdit->setReadOnly(false);
}
