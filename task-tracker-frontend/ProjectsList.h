#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringListModel>
#include <memory>
#include <QListWidgetItem>
#include "CreateProjectDialog.h"
#include "Backend.h"

namespace Ui {
class ProjectsList;
}

class ProjectsList : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectsList(QWidget *parent = nullptr);
    ~ProjectsList();

    void ChangeHeader(const QString& header);

    void SetProjects(const QList<QPair<ProjectInfo, RoleInfo>>& list);

signals:
    void ProjectSelected(const ProjectInfo& projectInfo, const RoleInfo& role);

    void AddProjectClicked(const QString& name);

private slots:
    void OnAddProjectBtnClicked();

    void OnProjectCreated(QString& projectName);

    void IfTextChanged();

    void OnItemClicked(QListWidgetItem*);
private:
    Ui::ProjectsList *ui;

    QList<QPair<ProjectInfo, RoleInfo>> myProjects;

    CreateProjectDialog* dialog;
};

#endif // PROJECTSLIST_H
