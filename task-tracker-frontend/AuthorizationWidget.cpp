#include "AuthorizationWidget.h"
#include "ui_AuthorizationWidget.h"
#include "Backend.h"

AuthorizationWidget::AuthorizationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizationWidget)
{
    ui->setupUi(this);
    connect(ui->LogInBtn, SIGNAL(clicked()), this, SLOT(OnLogInBtnClicked()));
    connect(ui->MoveToSignUpBtn, SIGNAL(clicked()), this, SLOT(OnMoveToSignUpBtnClicked()));

    connect(&Backend::Instance, &Backend::SignedIn, this, &AuthorizationWidget::OnLogin);
}

AuthorizationWidget::~AuthorizationWidget()
{
    delete ui;
}

void AuthorizationWidget::OnLogInBtnClicked() {
    Backend::Instance.SignIn(ui->usernameField->toPlainText(), ui->passwordField->toPlainText());
}

void AuthorizationWidget::OnMoveToSignUpBtnClicked() {
    emit AuthClicked(AuthorizationWindow::Transition::Registration);
}

void AuthorizationWidget::OnLogin(Status status)
{
    if (status.isSuccess) {
        emit LoggedIn();
        return;
    }

    // TODO: handle errors
}

