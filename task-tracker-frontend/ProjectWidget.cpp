#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"
#include "Backend.h"
#include <QListWidget>
#include <QDebug>
#include "MainWindow.h"
#include <QScrollBar>
#include <QSet>
#include <QComboBox>
#include <QtAlgorithms>

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

    connect(ui->searchInput, SIGNAL(textChanged()), this, SLOT (IfTextChanged()));

    connect(&Backend::Instance, &Backend::TaskEdited, this, &ProjectWidget::OnTaskUpdate);

    connect(&Backend::Instance, &Backend::ProjectUsersLoaded, this, &ProjectWidget::OnTeamLoaded);

    connect(&Backend::Instance, &Backend::TagCaptionsLoaded, this, &ProjectWidget::OnTagsLoaded);

    connect(ui->createdByBox, &QComboBox::currentIndexChanged, this, &ProjectWidget::OnFilterRequested);
    connect(ui->asigneeByBox, &QComboBox::currentIndexChanged, this, &ProjectWidget::OnFilterRequested);
    connect(ui->updateByBox, &QComboBox::currentIndexChanged, this, &ProjectWidget::OnFilterRequested);
    connect(ui->sortingBox, &QComboBox::currentIndexChanged, this, &ProjectWidget::OnFilterRequested);
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
    myContext.SetTask(currentTaskList[currentTaskList.count() - index.row() - 1]);
    emit TransitionRequested(MainWindow::Transition::Issue, myContext);
}

void ProjectWidget::OnTasksLoaded(Status status, const QList<TaskInfo> &tasks) {
    taskList.clear();

    for (auto& task : tasks){
        taskList.append(task);
    }

    SetupTaskFiltrage();

    DisplayTasks(taskList);
}

void ProjectWidget::IfTextChanged() {

    QStringList query_parts = ui->searchInput->toPlainText().toLower().trimmed().split(" ", Qt::SkipEmptyParts, Qt::CaseInsensitive);

    for(int i = 0; i < ui->listWidget->count(); i++){//this->taskList.size()

        QStringList title_parts =
                this->taskList[i].GetTitle().toLower().trimmed().split(" ", Qt::SkipEmptyParts, Qt::CaseInsensitive);

        bool is_title_contains_query = true;

        for (int k = 0; k < query_parts.size(); k++){

            bool is_query_part_find = false;

            for (int j = 0; j < title_parts.size(); j++){

                if (title_parts[j].indexOf(query_parts[k]) == 0) {
                    is_query_part_find = true;
                }

            }

            if (!is_query_part_find) {
                is_title_contains_query = false;
                break;
            }

        }
        ui->listWidget->item(ui->listWidget->count() - 1 - i)->setHidden(!is_title_contains_query);
        update();
    }
}

void ProjectWidget::OnTaskUpdate(Status status) {
    if (status.isSuccess) {
        Backend::Instance.GetTasks(myProject);
    }
}

void ProjectWidget::SetupPage() {
    myProject = myContext.GetCurrentProject();

    ui->ProjectNameLabel->setText(myProject.GetTitle());
    Backend::Instance.GetTasks(myProject);
    Backend::Instance.GetProjectUsers(myProject);
    //Backend::Instance.GetTagCaptions(myProject);
    //Backend::Instance.GetRoles(myProject);
    SetupSorting();
}

void ProjectWidget::SetupTaskFiltrage() {
//    QStringList fullNames = {"Created by"};
//    for (auto& el : taskList) {
//        auto newUsername = el.Get()->GetFullName();
//        if (std::find(fullNames.begin(), fullNames.end(), newUsername) == fullNames.end())
//            fullNames.append(newUsername);
//    }
//    ui->createdByBox->addItems(fullNames);

}

void ProjectWidget::OnTeamLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>> &team){
    myContext.SetProjectTeam(team);

    SetupTeamFiltrage();
}

void ProjectWidget::SetupTeamFiltrage() {
    ui->createdByBox->clear();
    ui->asigneeByBox->clear();
    ui->updateByBox->clear();
    QStringList fullNames = {"Created by"};
    for (auto& el : myContext.GetProjectTeam()) {
        auto newUsername = el.first.GetFullName();
        if (std::find(fullNames.begin(), fullNames.end(), newUsername) == fullNames.end())
            fullNames.append(newUsername);
    }
    ui->createdByBox->addItems(fullNames);
    fullNames[0] = "Asignee by";
    ui->asigneeByBox->addItems(fullNames);
    fullNames[0] = "Update by";
    ui->updateByBox->addItems(fullNames);

}

void ProjectWidget::OnFilterRequested(int index) {
    if (index == -1)
        return;
    QList<TaskInfo> filteredTasks;
    if (ui->createdByBox->currentIndex() > 0) {
        auto creator = ui->createdByBox->currentText();

        for (auto task : taskList) {
            if (task.GetCreator()->GetFullName() == creator)
                filteredTasks.append(task);
        }
    }
    if (ui->asigneeByBox->currentIndex() > 0) {
        auto asigner = ui->asigneeByBox->currentText();
        if (ui->createdByBox->currentIndex() < 1){
            for (auto task : taskList) {
                if (task.GetAssignee().has_value() && task.GetAssignee()->GetFullName() == asigner)
                    filteredTasks.append(task);
            }
        }
        else
        {
            QList<TaskInfo> newFilteredTasks;
            for (auto task : filteredTasks) {
                if (task.GetAssignee().has_value() && task.GetAssignee()->GetFullName() == asigner)
                    newFilteredTasks.append(task);
            }
            filteredTasks = newFilteredTasks;
        }
    }
    if (ui->updateByBox->currentIndex() > 0) {
        auto updater = ui->updateByBox->currentText();
        if (ui->createdByBox->currentIndex() < 1 && ui->asigneeByBox->currentIndex() < 1){
            for (auto task : taskList) {
                if (task.GetUpdater()->GetFullName() == updater)
                    filteredTasks.append(task);
            }
        }
        else {
            QList<TaskInfo> newFilteredTasks;
            for (auto task : taskList) {
                if (task.GetUpdater()->GetFullName() == updater)
                    newFilteredTasks.append(task);
            }
            filteredTasks = newFilteredTasks;
        }
    }
    if (ui->updateByBox->currentIndex() < 1 && ui->asigneeByBox->currentIndex() < 1
            && ui->createdByBox->currentIndex() < 1)
        DisplayTasks(SortTasks(taskList));
    else
        DisplayTasks(SortTasks(filteredTasks));
}

void ProjectWidget::DisplayTasks(const QList<TaskInfo> &tasks){
    ui->listWidget->clear();
    currentTaskList.clear();
    for (auto& task : tasks) {
        //TODO: change
        auto item = new QListWidgetItem();
        auto widget = new TaskItemWidget(this);
        widget->SetTask(task);
        item->setSizeHint(QSize(450, 60));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
        currentTaskList.append(task);
        update();
    }
}

QList<TaskInfo> ProjectWidget::SortTasks(const QList<TaskInfo> &tasks) {
    QList<TaskInfo> res = tasks;
    switch(ui->sortingBox->currentIndex()){
    case 1:
        std::sort(res.begin(), res.end(), [](const TaskInfo &left, const TaskInfo &right)
                  {return left.GetCreationTime() > right.GetCreationTime();} );
        break;
    case 2:
        std::sort(res.begin(), res.end(), [](const TaskInfo &left, const TaskInfo &right)
                  {return left.GetUpdateTime() > right.GetUpdateTime();} );
        break;
    case 3:
        std::sort(res.begin(), res.end(), [](const TaskInfo &left, const TaskInfo &right)
                  {return left.GetDeadline() > right.GetDeadline();} );
        break;
    case 4:
        std::sort(res.begin(), res.end(), [](const TaskInfo &left, const TaskInfo &right)
                  {return left.GetStoryPoints() > right.GetStoryPoints();} );
        break;
    default:
        std::sort(res.begin(), res.end(), [](const TaskInfo &left, const TaskInfo &right)
        {return left.GetCreationTime() > right.GetCreationTime();} );
        break;
    }
    return res;
}

void ProjectWidget::OnTagsLoaded(Status status, const QList<TagInfo>& tags) {
    auto deb = tags;
}

void ProjectWidget::SetupSorting() {
    QStringList sortNames = {"Sort by", "Creation time", "Update time", "Deadline", "Story points"};
    ui->sortingBox->clear();
    ui->sortingBox->addItems(sortNames);
}
