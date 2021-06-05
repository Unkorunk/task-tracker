#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QWidget>
#include "DataClasses.h"
#include "Backend.h"

namespace Ui {
class ProfileWidget;
}

class ProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWidget(QWidget *parent = nullptr);
    ~ProfileWidget();

    void SetupProfile(const UserInfo& user);

    void LoadAvatar(const std::string &strAvatarUrl);

signals:
    void Logout();

private slots:
    void OnEditUserInfoBtnClicked();
    void OnResetPasswordBtnClicked();
    void OnCheck(Status status);
    void OnDeleteAccountBtnClicked();
    //void OnUserDeleted(Status status);

private:
    Ui::ProfileWidget *ui;
    UserInfo user_info;
};

#endif // PROFILEWIDGET_H
