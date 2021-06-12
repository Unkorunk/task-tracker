#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"
#include "Backend.h"
#include <QListWidget>
#include <QDebug>
#include "MainWindow.h"
#include <QScrollBar>

ProjectWidget::ProjectWidget(QWidget *parent) :
    AbstractPage(parent),
    ui(new Ui::ProjectWidget),
    myProject(-1, "") {
    ui->setupUi(this);

    ui->listWidget->verticalScrollBar()->setSingleStep(2);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));

    connect(ui->CreateTaskBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnCreateTaskBtnClicked);

    connect(&Backend::Instance, &Backend::TasksLoaded, this, &ProjectWidget::OnTasksLoaded);

    connect(ui->ProjectSettingsBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnProjectSettingsBtnClicked);
    connect(ui->ProjectStatisticsBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnProjectStatisticsBtnClicked);

    connect(&Backend::Instance, &Backend::TaskEdited, this, &ProjectWidget::OnTaskUpdate);

    connect(&Backend::Instance, &Backend::ProjectUsersLoaded, this, &ProjectWidget::OnTeamLoaded);

    connect(ui->createdByBox, &QComboBox::currentIndexChanged, this, &ProjectWidget::OnCreatorRequested);
}

ProjectWidget::~ProjectWidget() {
    delete ui;
}

void ProjectWidget::OnCreateTaskBtnClicked() {
    Backend::Instance.CreateTask(TaskInfo(0, myProject, "New task", "Default task description",
                                          myContext.GetUser(), QDateTime::currentDateTime(), 0));
}

void ProjectWidget::OnProjectSettingsBtnClicked() {
    emit TransitionRequested(MainWindow::Transition::ProjectSettings, myContext);
}

void ProjectWidget::OnProjectStatisticsBtnClicked() {
    emit TransitionRequested(MainWindow::Transition::ProjectStatistics, myContext);
}

void ProjectWidget::OnItemClicked(QListWidgetItem* item) {
    auto index = ui->listWidget->indexFromItem(item);
    myContext.SetTask(taskList[taskList.count() - index.row() - 1]);
    emit TransitionRequested(MainWindow::Transition::Issue, myContext);
}

void ProjectWidget::OnTasksLoaded(Status status, const QList<TaskInfo> &tasks) {
    taskList.clear();
    for (auto& task : tasks){
        taskList.append(task);
    }
    DisplayTasks(taskList);
}

void ProjectWidget::OnTaskUpdate(Status status) {
    if (status.isSuccess) {
        Backend::Instance.GetTasks(myProject);
    }

    // TODO: handle errors
}

void ProjectWidget::SetupPage() {
    myProject = myContext.GetCurrentProject();

    ui->ProjectNameLabel->setText(myProject.GetTitle());
    Backend::Instance.GetTasks(myProject);
    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectWidget::SetupFiltrage() {
    auto pr_team = myContext.GetProjectTeam();
    QStringList fullNames = {"Created by"};
    for (auto& el : pr_team) {
        fullNames.append(el.first.GetFullName());
    }
    ui->createdByBox->clear();
    ui->createdByBox->addItems(fullNames);
}

void ProjectWidget::OnTeamLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>> &team){
    myContext.SetProjectTeam(team);

    SetupFiltrage();
}

void ProjectWidget::OnCreatorRequested(int index) {
    if (index <= 0 || ui->createdByBox->currentIndex() <= 0) return;

    QList<TaskInfo> filteredTasks;

    auto creator = ui->createdByBox->currentText();

    for (auto task : taskList) {
        if (task.GetCreator()->GetFullName() == creator)
            filteredTasks.append(task);
    }

    DisplayTasks(filteredTasks);
}

void ProjectWidget::DisplayTasks(const QList<TaskInfo> &tasks){
    ui->listWidget->clear();
    for (auto& task : tasks) {
        //TODO: change
        auto item = new QListWidgetItem();
        auto widget = new TaskItemWidget(this);
        widget->SetTask(task);
        item->setSizeHint(QSize(450, 60));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);

        update();
    }
}
