#include "IssueWidget.h"
#include "ui_IssueWidget.h"

IssueWidget::IssueWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IssueWidget),
    task_info(0, 0, "Task name", "Task description")
{
    this->ui->setupUi(this);
    this->SetupTask(task_info);
    this->edited = false;

    this->ui->saveButton->setVisible(false);
    this->ui->textEdit->setReadOnly(true);

    connect(ui->editButton, &QPushButton::clicked, this, &IssueWidget::OnEdit);
    connect(ui->saveButton, &QPushButton::clicked, this, &IssueWidget::OnSave);
}

IssueWidget::~IssueWidget()
{
    delete ui;
}

void IssueWidget::OnEdit(){
    if (this->edited){
        this->edited = false;
        this->ui->editButton->setText("Edit");
        this->ui->saveButton->setVisible(false);
        //lock objects
        this->ui->textEdit->setText(task_info.taskDescription);
        this->ui->textEdit->setReadOnly(true);
        // lock task name

    } else {
        this->edited = true;
        this->ui->editButton->setText("Cancel");
        this->ui->saveButton->setVisible(true);
        //unclock objects
        this->ui->textEdit->setReadOnly(false);
        //unlock task name
    }
}

void IssueWidget::OnSave(){
    //save
    task_info.taskDescription = this->ui->textEdit->toPlainText();
    this->ui->textEdit->setText(task_info.taskDescription);
    //and lock
    this->edited = false;
    this->ui->textEdit->setReadOnly(true);
    this->ui->editButton->setText("Edit");
    this->ui->saveButton->setVisible(false);

}

void IssueWidget::SetupTask(const TaskInfo &task)
{
    this->ui->label->setText(task.taskName);
    this->ui->textEdit->setText(task.taskDescription);
    //set (task.team)
}
