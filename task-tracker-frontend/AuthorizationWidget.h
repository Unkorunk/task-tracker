#ifndef AUTHORIZATIONWIDGET_H
#define AUTHORIZATIONWIDGET_H

#include <QWidget>
#include "AuthorizationWindow.h"

namespace Ui {
class AuthorizationWidget;
}

class AuthorizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorizationWidget(QWidget *parent = nullptr);
    ~AuthorizationWidget();

private slots:
    void OnLogInBtnClicked();
    void OnMoveToSignUpBtnClicked();
    void OnLogin(Status status, const UserInfo& user);
signals:
    void FailAuthorization(QString log);
    void LoggedIn(const UserInfo& user);
    void AuthClicked(AuthorizationWindow::Transition transition);

private:
    Ui::AuthorizationWidget *ui;
};

#endif // AUTHORIZATIONWIDGET_H
