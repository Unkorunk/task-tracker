#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>
#include "AbstractPage.h"
#include "Backend.h"
#include "DateTimePropertyWidget.h"
#include "IntegerSelectorWidget.h"
#include "MainWindow.h"
#include "UserSelectorWidget.h"

namespace Ui {
class IssueWidget;
}

class IssueWidget : public AbstractPage
{
    Q_OBJECT
public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

signals:
    void TaskDeleted(MainWindow::Transition transition, const Context& context);

protected:
    virtual void SetupPage() override;


private slots:
    void OnEditClicked();
    void OnSaveClicked();
    void OnDeleteClicked();

    void OnTaskUpdated(Status status, const TaskInfo& task);
    void OnTaskDeleted(Status status);

private:
    void ToEditMode();
    void ToReadOnlyMode();

    void LockUi();
    void UnlockUi();

    void AddPropertyItem(QWidget* propertyWidget);

    Ui::IssueWidget *ui;
    bool edited;

    UserSelectorWidget* myUpdaterSelector;
    DateTimePropertyWidget* myUpdateTimeSelector;
    DateTimePropertyWidget* myDeadlineSelector;
    IntegerSelectorWidget* myStorypointSelector;
    UserSelectorWidget* myAssigneeSelector;
    //QString text;
};

#endif // ISSUEWIDGET_H
