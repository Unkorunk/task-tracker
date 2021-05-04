#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include "backend.h"
#include <QStandardItemModel>
#include <QStringListModel>
#include <memory>

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
    void TaskSelected(const ProjectInfo& projectInfo);

    void CreateTaskClicked(const QString& name);

private slots:
    void OnCreateTaskBtnClicked();

    void OnItemClicked(const QModelIndex& index);
private:
    Ui::ProjectWidget *ui;

    std::unique_ptr<QStringListModel> model;

    QList<ProjectInfo> myTasks;

  //  std::unique_ptr<QDialog> myDialog;
};

#endif // PROJECTWIDGET_H
