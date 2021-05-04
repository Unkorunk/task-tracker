#include "AuthorizationWindow.h"
#include "MainWindow.h"
#include "ui_AuthorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QMainWindow(parent),
    mWindow(*this),
    ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);
    connect(ui->Authorization, SIGNAL(OnAuthClicked(AuthorizationWindow::Transition)), this, SLOT(OnTransition(AuthorizationWindow::Transition)));
    connect(ui->Registration, SIGNAL(OnRegClicked(AuthorizationWindow::Transition)), this, SLOT(OnTransition(AuthorizationWindow::Transition)));
    connect(ui->Authorization, SIGNAL(OnLoggedIn()), this, SLOT(OnSwitchingWindow()));
    connect(ui->Registration, SIGNAL(OnSignedUp()), this, SLOT(OnSwitchingWindow()));
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
}
