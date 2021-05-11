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

    connect(&Backend::Instance, &Backend::SignedUp, this, &RegistrationWidget::OnSignup);
}

RegistrationWidget::~RegistrationWidget()
{
    delete ui;
}

void RegistrationWidget::OnSignUpBtnClicked() {
    QString username = ui->usernameField->text();
    QString email = ui->emailField->text();
    QString password = ui->passwordField->text();

    if (password != ui->repPasswordField->text() || password.length() < 6 || username.length() == 0 || email.length() == 0) {
        // TODO: handle this
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
    ui->fullNameField->setText("");
    ui->emailField->setText("");
    ui->usernameField->setText("");
    ui->passwordField->setText("");
    ui->repPasswordField->setText("");
    emit SignupBtnClicked(AuthorizationWindow::Transition::Authorization);
}

void RegistrationWidget::OnSignup(Status status)
{
    ui->fullNameField->setReadOnly(false);
    ui->emailField->setReadOnly(false);
    ui->usernameField->setReadOnly(false);
    ui->passwordField->setReadOnly(false);
    ui->repPasswordField->setReadOnly(false);

    if (status.isSuccess) {
        emit SignedUp();
        //emit SignupBtnClicked(AuthorizationWindow::Transition::Authorization);
        ui->fullNameField->setText("");
        ui->emailField->setText("");
        ui->usernameField->setText("");
        ui->passwordField->setText("");
        ui->repPasswordField->setText("");

        return;
    }

    // TODO: handle this
}
