#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringListModel>
#include <memory>

#include "Backend.h"

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    ~ProjectWidget();

    void SetupProject(const ProjectInfo& project);

signals:
    void TaskSelected(const TaskInfo& taskInfo);

    void ProjectSettingsClicked(const ProjectInfo& projectInfo);

    void ProjectStatisticsClicked(const ProjectInfo& projectInfo);

    void CreateTaskClicked(const QString& name);

private slots:
    void OnCreateTaskBtnClicked();

    void OnProjectSettingsBtnClicked();

    void OnProjectStatisticsBtnClicked();

    void OnItemClicked(const QModelIndex& index);

    void OnTasksLoaded(Status status, const QList<TaskInfo>& tasks);

    void OnTaskUpdate(Status status);
private:
    Ui::ProjectWidget *ui;

    std::unique_ptr<QStringListModel> model;

    QList<TaskInfo> myTasks;
    ProjectInfo myProject;

  //  std::unique_ptr<QDialog> myDialog;
};

#endif // PROJECTWIDGET_H
