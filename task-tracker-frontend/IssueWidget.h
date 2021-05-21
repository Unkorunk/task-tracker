#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>
#include "Backend.h"

namespace Ui {
class IssueWidget;
}

class IssueWidget : public QWidget
{
    Q_OBJECT

private slots:
    void OnEditClicked();
    void OnSaveClicked();
    void OnDeleteClicked();

    void OnTaskUpdated(Status status);
    void OnTaskDeleted(Status status);

public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

    void SetupTask(const ProjectInfo& project, const TaskInfo& task);

private:
    void ToEditMode();
    void ToReadOnlyMode();

    void LockUi();
    void UnlockUi();

    void AddPropertyItem(QWidget* propertyWidget);

    Ui::IssueWidget *ui;
    bool edited;
    TaskInfo task_info;
    ProjectInfo project_info;
    //QString text;
};

#endif // ISSUEWIDGET_H
