#ifndef USERITEMWIDGET_H
#define USERITEMWIDGET_H

#include "Backend.h"
#include "DataClasses.h"

#include <QList>
#include <QWidget>

namespace Ui {
class UserItemWidget;
}

class UserItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserItemWidget(QWidget *parent = nullptr);
    ~UserItemWidget();

    void SetRoles(const UserInfo& user, const QList<RoleInfo>& roles, const ProjectInfo& projectInfo, int roleId);

private slots:
    void OnRoleChangeRequested();

    void OnRoleChanged(Status status);

    void OnKickClicked();

private:
    Ui::UserItemWidget *ui;

    UserInfo myUser;
    ProjectInfo myProject;
    QList<RoleInfo> myRoles;
};
#endif // USERITEMWIDGET_H
