#include "AuthorizationWindow.h"
#include "MainWindow.h"
#include "ui_AuthorizationWindow.h"
#include "Backend.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QMainWindow(parent),
    mWindow(*this),
    ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);
    connect(ui->Authorization, &AuthorizationWidget::AuthClicked, this, &AuthorizationWindow::OnTransition);
    connect(ui->Registration, &RegistrationWidget::SignupBtnClicked, this, &AuthorizationWindow::OnTransition);
    connect(ui->Authorization, &AuthorizationWidget::LoggedIn, this, &AuthorizationWindow::OnSwitchingWindow);
    connect(ui->Registration, &RegistrationWidget::SignedUp, this, &AuthorizationWindow::OnSwitchingWindow);
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}

void AuthorizationWindow::OnTransition(AuthorizationWindow::Transition transition)
{
    ui->stackedWidget->setCurrentIndex((int)transition);
}

void AuthorizationWindow::OnSwitchingWindow() {
    mWindow.show();
    this->close();

    Backend::Instance.UpdateProfile();
    Backend::Instance.GetProjects();
}
