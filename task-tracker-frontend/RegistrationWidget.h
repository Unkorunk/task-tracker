#ifndef REGISTRATIONWIDGET_H
#define REGISTRATIONWIDGET_H

#include <QWidget>
#include "AuthorizationWindow.h"

namespace Ui {
class RegistrationWidget;
}

class RegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationWidget(QWidget *parent = nullptr);
    ~RegistrationWidget();

private slots:
    void OnSignUpBtnClicked();
    void OnMoveToLogInBtnClicked();
    void OnSignup(Status status);
signals:
    void SignedUp();
    void SignupBtnClicked(AuthorizationWindow::Transition transition);

private:
    Ui::RegistrationWidget *ui;
};

#endif // REGISTRATIONWIDGET_H
