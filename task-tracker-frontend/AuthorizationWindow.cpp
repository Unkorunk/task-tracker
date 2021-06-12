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
    ui->stackedWidget->setStyleSheet("QStackedWidget#stackedWidget {background-color: rgb(232, 227, 227); border-radius: 20px;}");
    ui->centralwidget->setStyleSheet("QWidget#centralwidget {background-color: rgb(255, 255, 255);}");
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

void AuthorizationWindow::OnSwitchingWindow(const UserInfo& user) {
    mWindow.show();
    this->close();

    Context context;
    context.SetUser(user);
    mWindow.OnTransition(MainWindow::Transition::Greetings, context);

    Backend::Instance.UpdateProfile();

    Backend::Instance.GetProjects();
}
