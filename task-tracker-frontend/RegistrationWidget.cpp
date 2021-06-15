#include "RegistrationWidget.h"
#include "ui_RegistrationWidget.h"
#include "Backend.h"

RegistrationWidget::RegistrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWidget)
{
    ui->setupUi(this);
    connect(ui->SignUpBtn, SIGNAL(clicked()), this, SLOT(OnSignUpBtnClicked()));
    connect(ui->MoveToLogInBtn, SIGNAL(clicked()), this, SLOT(OnMoveToLogInBtnClicked()));
    ui->loadingBar->hide();
    connect(&Backend::Instance, &Backend::SignedUp, this, &RegistrationWidget::OnSignup);
}

RegistrationWidget::~RegistrationWidget()
{
    delete ui;
}

void RegistrationWidget::OnSignUpBtnClicked() {

    ui->loadingBar->show();
    ui->loadingBar->StartLoading();

    QString username = ui->usernameField->text();
    QString email = ui->emailField->text();
    QString password = ui->passwordField->text();

    if (ui->fullNameField->text().isEmpty()){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Не заполнено полное имя пользователя!");
        return;
    }
    else if (ui->usernameField->text().isEmpty()){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Не заполнен логин!");
        return;
    }
    else if (ui->emailField->text().isEmpty()){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Не указан адрес электронной почты!");
        return;
    }
    else if (!ui->emailField->text().contains('@') || !ui->emailField->text().contains('.')){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Некорректный адрес электронной почты!");
        return;
    }
    else if (ui->passwordField->text().isEmpty()){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Не заполнен пароль!");
        return;
    }
    else if (ui->passwordField->text().length() < 6){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Длина пароля должна быть не меньше 6 символов!");
        return;
    }
    else if (ui->repPasswordField->text().isEmpty()){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Не введён дубликат пароля!");
        return;
    }
    else if (ui->passwordField->text() != ui->repPasswordField->text()){
        ui->loadingBar->show();
        emit Backend::Instance.RequestFailed("Повторно введённый пароль не совпадает с исходным.");
        return;
    }

    ui->fullNameField->setReadOnly(true);
    ui->emailField->setReadOnly(true);
    ui->usernameField->setReadOnly(true);
    ui->passwordField->setReadOnly(true);
    ui->repPasswordField->setReadOnly(true);

    Backend::Instance.SignUp(ui->fullNameField->text(), username, email, password);
}

void RegistrationWidget::OnMoveToLogInBtnClicked() {

    ui->loadingBar->hide();

    ui->fullNameField->setText("");
    ui->emailField->setText("");
    ui->usernameField->setText("");
    ui->passwordField->setText("");
    ui->repPasswordField->setText("");
    ui->loadingBar->StopLoading();
    emit SignupBtnClicked(AuthorizationWindow::Transition::Authorization);
}

void RegistrationWidget::OnSignup(Status status, const UserInfo& user)
{

    ui->loadingBar->show();
    ui->loadingBar->StartLoading();

    ui->fullNameField->setReadOnly(false);
    ui->emailField->setReadOnly(false);
    ui->usernameField->setReadOnly(false);
    ui->passwordField->setReadOnly(false);
    ui->repPasswordField->setReadOnly(false);

    if (status.isSuccess) {
        ui->loadingBar->StopLoading();
        emit SignedUp(user);
        ui->fullNameField->setText("");
        ui->emailField->setText("");
        ui->usernameField->setText("");
        ui->passwordField->setText("");
        ui->repPasswordField->setText("");

        return;
    } else{
        if (ui->fullNameField->text().isEmpty()){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Не заполнено полное имя пользователя!");
        }
        else if (ui->usernameField->text().isEmpty()){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Не заполнен логин!");
        }
        else if (ui->emailField->text().isEmpty()){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Не указан адрес электронной почты!");
        }
        else if (!ui->emailField->text().contains('@') && !ui->emailField->text().contains('.')){
            emit Backend::Instance.RequestFailed("Некорректный адрес электронной почты!");
        }
        else if (ui->passwordField->text().isEmpty()){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Не заполнен пароль!");
        }
        else if (ui->passwordField->text().length() < 6){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Длина пароля должна быть не меньше 6 символов!");
        }
        else if (ui->repPasswordField->text().isEmpty()){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Не введён дубликат пароля!");
        }
        else if (ui->passwordField->text() != ui->repPasswordField->text()){
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Повторно введённый пароль не совпадает с исходным.");
        } else {
            ui->loadingBar->show();
            emit Backend::Instance.RequestFailed("Пользователь с данными логином/электронной почтой уже существует!");
        }
    }
}
