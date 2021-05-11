#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"
#include "Backend.h"
#include <QListWidget>
#include <QDebug>
#include "MainWindow.h"

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget),
    myProject(0, 0, "")
{
    ui->setupUi(this);

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));

    connect(ui->CreateTaskBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnCreateTaskBtnClicked);

    connect(&Backend::Instance, &Backend::TasksLoaded, this, &ProjectWidget::OnTasksLoaded);

    connect(this, &ProjectWidget::TaskSelected, MainWindow::Instance, &MainWindow::OnIssueTransition);

    connect(ui->ProjectSettingsBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnProjectSettingsBtnClicked);
    connect(this, &ProjectWidget::ProjectSettingsClicked, MainWindow::Instance, &MainWindow::OnProjectSettingsTransition);

    connect(ui->ProjectStatisticsBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnProjectStatisticsBtnClicked);
    connect(this, &ProjectWidget::ProjectStatisticsClicked, MainWindow::Instance, &MainWindow::OnProjectStatisticsTransition);

    connect(&Backend::Instance, &Backend::TaskEdited, this, &ProjectWidget::OnTaskUpdate);
}

void ProjectWidget::RemoveItem(const QString &text)
{
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::OnCreateTaskBtnClicked()
{
    Backend::Instance.CreateTask(TaskInfo(0, myProject.projectId, QString("NewTask"), "Default task description"));

//    if (myDialog.get() != nullptr && myDialog->isVisible()) {
//        myDialog->close();
//    }

//    myDialog = std::make_unique<QDialog>(this->parent());
//    QVBoxLayout dialogLayout;
//    dialogLayout.addItem(new QTextBlock());

//    myDialog->setLayout()
    //   emit CreateTaskClicked(QString("NewTask%1").arg(model->rowCount()));


}

void ProjectWidget::OnProjectSettingsBtnClicked()
{
    emit ProjectSettingsClicked(myProject);
}

void ProjectWidget::OnProjectStatisticsBtnClicked()
{

    emit ProjectSettingsClicked(myProject);
}

void ProjectWidget::OnItemClicked(QListWidgetItem* item)
{
    auto index = ui->listWidget->indexFromItem(item);
    qInfo() << index.row();
    qInfo() << taskList[taskList.count() - index.row() - 1].taskName;
    emit TaskSelected(myProject, taskList[taskList.count() - index.row() - 1]);
}

void ProjectWidget::OnTasksLoaded(Status status, const QList<TaskInfo> &tasks)
{
    ui->listWidget->clear();
    for (auto& task : tasks) {
        //TODO: change
        taskList.append(task);
        auto item = new QListWidgetItem();
        auto widget = new TaskItemWidget(this);
        widget->setTask(task.taskName);
        item->setSizeHint(QSize(200, 100));

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
        update();
    }
}

void ProjectWidget::OnTaskUpdate(Status status)
{
    if (status.isSuccess) {
        Backend::Instance.GetTasks(myProject);
    }

    // TODO: handle errors
}


void ProjectWidget::SetupProject(const ProjectInfo &project)
{
    myProject = project;

    ui->ProjectNameLabel->setText(myProject.projectName);

    Backend::Instance.GetTasks(myProject);
}
