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

    void SetProjects(const QList<ProjectInfo>& list);

signals:
    void ProjectSelected(const ProjectInfo& projectInfo);

    void AddProjectClicked(const QString& name);

private slots:
    void OnAddProjectBtnClicked();

    void OnProjectCreated(QString& projectName);

    void OnItemClicked(QListWidgetItem*);
private:
    Ui::ProjectsList *ui;

    QList<ProjectInfo> myProjects;

    CreateProjectDialog* dialog;
};

#endif // PROJECTSLIST_H
