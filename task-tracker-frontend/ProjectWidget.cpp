#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"
#include "Backend.h"

#include <QDebug>
#include "MainWindow.h"

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget),
    myProject(0, 0, ""),
    model(std::make_unique<QStringListModel>())
{
    ui->setupUi(this);

    ui->listView->setModel(model.get());

    connect(ui->CreateTaskBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnCreateTaskBtnClicked);
    connect(ui->listView, &QListView::clicked, this, &ProjectWidget::OnItemClicked);
    connect(&Backend::Instance, &Backend::TasksLoaded, this, &ProjectWidget::OnTasksLoaded);

    connect(this, &ProjectWidget::TaskSelected, MainWindow::Instance, &MainWindow::OnIssueTransition);

    connect(ui->ProjectSettingsBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnProjectSettingsBtnClicked);
    connect(this, &ProjectWidget::ProjectSettingsClicked, MainWindow::Instance, &MainWindow::OnProjectSettingsTransition);

    connect(ui->ProjectStatisticsBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnProjectStatisticsBtnClicked);
    connect(this, &ProjectWidget::ProjectStatisticsClicked, MainWindow::Instance, &MainWindow::OnProjectStatisticsTransition);

    connect(&Backend::Instance, &Backend::TaskEdited, this, &ProjectWidget::OnTaskUpdate);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::OnCreateTaskBtnClicked()
{
    Backend::Instance.CreateTask(TaskInfo(model->rowCount(), myProject.projectId, QString("NewTask"), "Default task description"));

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

void ProjectWidget::OnItemClicked(const QModelIndex &index)
{
    emit TaskSelected(myTasks[index.row()]);
}

void ProjectWidget::OnTasksLoaded(Status status, const QList<TaskInfo> &tasks)
{
    myTasks = tasks;

    QStringList list;

    for (auto& task : tasks) {
        list.append(task.taskName);
    }

    model->setStringList(list);
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
