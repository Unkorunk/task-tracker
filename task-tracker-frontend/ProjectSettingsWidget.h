#ifndef PROJECTSETTINGSWIDGET_H
#define PROJECTSETTINGSWIDGET_H

#include <QWidget>
#include "AbstractPage.h"
#include "Backend.h"
#include "MainWindow.h"

namespace Ui {
class ProjectSettingsWidget;
}

class ProjectSettingsWidget : public AbstractPage
{
    Q_OBJECT

private slots:
    void OnSaveClicked();
    void OnCancelClicked();

    void OnProjectEdited(Status status);
    void OnDeleteClicked();
    void OnProjectDeleted(Status status);

    void OnRoleCreateClicked();

    void OnRolesLoaded(Status status, const QList<RoleInfo>& roles);
    void OnRoleCreated(Status status, const RoleInfo& role);
    void OnRoleEdited(Status status, const RoleInfo& role);
    void OnRoleDeleted(Status status);

    void OnInviteClicked();
    void OnMemberInvited(Status status);

    void OnMemberKicked(Status status);

    void OnUsersLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>>& users);

    void OnTagCreateClicked();

    void OnTagsLoaded(Status status, const QList<TagInfo>& tags);
    void OnTagDeleted(Status status);
    void OnTagCreated(Status status, const TagInfo& tag);
    void OnTagEdited(Status status, const TagInfo& tag);
signals:
    void TransitionRequested(MainWindow::Transition, const Context& context);
public:
    explicit ProjectSettingsWidget(QWidget *parent = nullptr);
    ~ProjectSettingsWidget();

protected:
    void SetupPage() override;

private:
    void ToEditMode();

    void ToReadonlyMode();

    void LockUi();

    void UnlockUi();

    Ui::ProjectSettingsWidget *ui;

    ProjectInfo myProject;
    QList<RoleInfo> myRoles;
};

#endif // PROJECTSETTINGSWIDGET_H
