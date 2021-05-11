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
    ui->fullNameField->setReadOnly(true);
    ui->emailField->setReadOnly(true);
    ui->usernameField->setReadOnly(true);
    ui->passwordField->setReadOnly(true);
    ui->repPasswordField->setReadOnly(true);

    QString username = ui->usernameField->toPlainText();
    QString email = ui->emailField->toPlainText();
    QString password = ui->passwordField->toPlainText();

    if (password != ui->repPasswordField->toPlainText() || password.length() < 6 || username.length() == 0 || email.length() == 0) {
        // TODO: handle this
        return;
    }

    Backend::Instance.SignUp(ui->fullNameField->toPlainText(), username, email, password);
}

void RegistrationWidget::OnMoveToLogInBtnClicked() {
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
        // TODO: instant login
        emit SignedUp();
        //emit SignupBtnClicked(AuthorizationWindow::Transition::Authorization);

        return;
    }

    // TODO: handle this
}
