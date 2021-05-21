#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>
#include "Backend.h"
#include "DateTimePropertyWidget.h"
#include "IntegerSelectorWidget.h"
#include "UserSelectorWidget.h"

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

    void OnTaskUpdated(Status status, const TaskInfo& task);
    void OnTaskDeleted(Status status);

public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

    void SetupTask(const ProjectInfo& project, const TaskInfo& task);

private:
    void ToEditMode();
    void ToReadOnlyMode();
    void InternalSetupTask(const TaskInfo& task);

    void LockUi();
    void UnlockUi();

    void AddPropertyItem(QWidget* propertyWidget);

    Ui::IssueWidget *ui;
    bool edited;
    TaskInfo task_info;
    ProjectInfo project_info;

    UserSelectorWidget* myUpdaterSelector;
    DateTimePropertyWidget* myUpdateTimeSelector;
    DateTimePropertyWidget* myDeadlineSelector;
    IntegerSelectorWidget* myStorypointSelector;
    UserSelectorWidget* myAssigneeSelector;
    //QString text;
};

#endif // ISSUEWIDGET_H
