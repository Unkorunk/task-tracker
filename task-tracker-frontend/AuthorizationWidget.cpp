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
    //connect(this, &AuthorizationWidget::FailAuthorization, )

    connect(&Backend::Instance, &Backend::SignedIn, this, &AuthorizationWidget::OnLogin);
}

AuthorizationWidget::~AuthorizationWidget()
{
    delete ui;
}

void AuthorizationWidget::OnLogInBtnClicked() {
    ui->usernameField->setReadOnly(true);
    ui->passwordField->setReadOnly(true);

    Backend::Instance.SignIn(ui->usernameField->text(), ui->passwordField->text());

    ui->loadingBar->StartLoading();

}

void AuthorizationWidget::OnMoveToSignUpBtnClicked() {
    ui->loadingBar->StopLoading();
    emit AuthClicked(AuthorizationWindow::Transition::Registration);
    ui->usernameField->setText("");
    ui->passwordField->setText("");
}

void AuthorizationWidget::OnLogin(Status status, const UserInfo& user)
{
    ui->usernameField->setReadOnly(false);
    ui->passwordField->setReadOnly(false);

    if (status.isSuccess) {
        emit LoggedIn(user);
        ui->usernameField->setText("");
        ui->passwordField->setText("");
        return;
    } else {
        if (ui->usernameField->text().isEmpty()){
            emit Backend::Instance.RequestFailed("Не заполнен логин!");
        }
        else if (ui->passwordField->text().isEmpty()) {
            emit Backend::Instance.RequestFailed("Не заполнен пароль!");
        }
    }
}

