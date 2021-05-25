#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"
#include "Backend.h"
#include <QListWidget>
#include <QDebug>
#include "MainWindow.h"
#include <QScrollBar>

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget),
    myProject(-1, "")
{
    ui->setupUi(this);

    ui->listWidget->verticalScrollBar()->setSingleStep(2);
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
    Backend::Instance.CreateTask(TaskInfo(0, myProject, "New task", "Default task description", Backend::Instance.GetProfile(), QDateTime::currentDateTime(), 0));
    MainWindow::Instance->StartLoading();
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
    emit TaskSelected(myProject, taskList[taskList.count() - index.row() - 1]);
}

void ProjectWidget::OnTasksLoaded(Status status, const QList<TaskInfo> &tasks)
{
    MainWindow::Instance->StopLoading();
    ui->listWidget->clear();
    for (auto& task : tasks) {
        //TODO: change
        taskList.append(task);
        auto item = new QListWidgetItem();
        auto widget = new TaskItemWidget(this);
        widget->SetTask(task);
        item->setSizeHint(QSize(450, 60));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);

        update();
    }
}

void ProjectWidget::OnTaskUpdate(Status status)
{
    MainWindow::Instance->StopLoading();
    if (status.isSuccess) {
        Backend::Instance.GetTasks(myProject);
    }

    // TODO: handle errors
}


void ProjectWidget::SetupProject(const ProjectInfo &project)
{
    myProject = project;

    ui->ProjectNameLabel->setText(myProject.GetTitle());

    MainWindow::Instance->StartLoading();
    Backend::Instance.GetTasks(myProject);
}
