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

    connect(ui->searchInput, SIGNAL(textChanged()), this, SLOT (IfTextChanged()));

    connect(&Backend::Instance, &Backend::TaskEdited, this, &ProjectWidget::OnTaskUpdate);
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
    ui->listWidget->clear();
    ui->searchInput->clear();

    this->taskList.clear();
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
}
