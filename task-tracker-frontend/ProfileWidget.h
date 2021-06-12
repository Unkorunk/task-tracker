#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include "AbstractPage.h"

#include <QWidget>
#include "DataClasses.h"
#include "Backend.h"

namespace Ui {
class ProfileWidget;
}

class ProfileWidget : public AbstractPage {
    Q_OBJECT

public:
    explicit ProfileWidget(QWidget *parent = nullptr);
    ~ProfileWidget();

    void SetupProfile(const UserInfo& user);

    void LoadAvatar(const std::string &strAvatarUrl);

signals:
    void Logout();
    //void ProfileUpdate();

private slots:
    void OnEditUserInfoBtnClicked();
    void OnResetPasswordBtnClicked();
    void OnCheck(Status status);
    void OnDeleteAccountBtnClicked();
    //void OnUserDeleted(Status status);
    void OnGetPicture(QNetworkReply* relpy);
protected:
    void SetupPage() override;

private:
    Ui::ProfileWidget *ui;
};

#endif // PROFILEWIDGET_H
