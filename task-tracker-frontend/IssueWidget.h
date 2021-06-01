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

    void OnCommentCreated(Status status, const CommentInfo& comment);
    void OnCommentDeleted(Status status);
    void OnCommentsLoaded(Status status, const QList<CommentInfo>& comments);

    void OnTagsLoaded(Status status, const QList<TagInfo>& tags);
    void OnTeamLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>>& team);

    void OnTagAdded(Status status, const TaskTag& taskTag);
    void OnTagRemoved(Status status);

private:
    void UpdateComments();
    void UpdateTags();

    void ToEditMode();
    void ToReadOnlyMode();

    void LockUi();
    void UnlockUi();

    void AddPropertyItem(QWidget* propertyWidget);

    Ui::IssueWidget *ui;
    bool edited;
    bool canEdit;

    UserSelectorWidget* myCreatorSelector;
    UserSelectorWidget* myUpdaterSelector;
    DateTimePropertyWidget* myUpdateTimeSelector;
    DateTimePropertyWidget* myDeadlineSelector;
    IntegerSelectorWidget* myStorypointSelector;
    UserSelectorWidget* myAssigneeSelector;
};

#endif // ISSUEWIDGET_H
