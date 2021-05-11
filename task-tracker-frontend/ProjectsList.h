#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringListModel>
#include <memory>

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

    void OnItemClicked(const QModelIndex& index);
private:
    Ui::ProjectsList *ui;

    std::unique_ptr<QStringListModel> model;

    QList<ProjectInfo> myProjects;

    std::unique_ptr<QDialog> myDialog;
};

#endif // PROJECTSLIST_H
