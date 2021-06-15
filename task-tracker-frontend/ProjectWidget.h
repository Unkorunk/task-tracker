#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QListWidgetItem>
#include <memory>
#include "TaskItemWidget.h"
#include "Backend.h"
#include "AbstractPage.h"
#include "MainWindow.h"
#include "MainWindow.h"

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public AbstractPage
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    ~ProjectWidget();

signals:
    void TransitionRequested(MainWindow::Transition transition, const Context& context);

    void CreateTaskClicked(const QString& name);

protected:
    void SetupPage() override;

private slots:
    void OnCreateTaskBtnClicked();

    void OnProjectSettingsBtnClicked();
    void OnProjectStatisticsBtnClicked();

    void OnItemClicked(QListWidgetItem*);

    void OnTasksLoaded(Status status, const QList<TaskInfo>& tasks);
    void OnTaskUpdate(Status status);

    void OnTeamLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>> &team);

    void OnFilterRequested(int index);

    void OnTagsLoaded(Status status, const QList<TagInfo>& tags);

private:
    void SetupTaskFiltrage();

    void SetupTeamFiltrage();

    void DisplayTasks(const QList<TaskInfo> &tasks);

    QList<TaskInfo> SortTasks(const QList<TaskInfo> &tasks);

    Ui::ProjectWidget *ui;

    ProjectInfo myProject;

    QList<TaskInfo> taskList;

    QList<TaskInfo> currentTaskList;
};

#endif // PROJECTWIDGET_H
