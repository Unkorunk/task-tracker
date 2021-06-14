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

    if (ui->usernameField->text().isEmpty()){
        //emit Backend::Instance.SignInFailed("Не заполнен логин!");
        //LoadingBar::FailLoading("Не заполнен логин!");
    }

    if (ui->passwordField->text().isEmpty()){
        // не надо ничего делать с бэком раз такая залупа emit Backend::Instance.SignInFailed("Не заполнен пароль!");
    }

    Backend::Instance.SignIn(ui->usernameField->text(), ui->passwordField->text());

    ui->loadingBar->StartLoading();
    ui->loadingBar->FailLoading("pizdec");

    if (false) {
        //если с бека пришла обама, то я прекращаю крутить и вывожу, что автор лох
    }
}

void AuthorizationWidget::OnMoveToSignUpBtnClicked() {
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
            emit Backend::Instance.SignInFailed("Пустое поле с логином");
        }
    }

    // TODO: handle errors
}

