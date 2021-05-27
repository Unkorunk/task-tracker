#ifndef PROJECTSETTINGSWIDGET_H
#define PROJECTSETTINGSWIDGET_H

#include <QWidget>
#include "AbstractPage.h"
#include "Backend.h"

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

    void OnRoleCreateClicked();

    void OnRolesLoaded(Status status, const QList<RoleInfo>& roles);
    void OnRoleCreated(Status status, const RoleInfo& role);
    void OnRoleEdited(Status status, const RoleInfo& role);
    void OnRoleDeleted(Status status);

    void OnInviteClicked();
    void OnMemberInvited(Status status);

    void OnMemberKicked(Status status);

    void OnUsersLoaded(Status status, const QList<QPair<UserInfo, RoleInfo>>& users);
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
