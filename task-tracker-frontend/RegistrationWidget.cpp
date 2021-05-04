#include "RegistrationWidget.h"
#include "ui_RegistrationWidget.h"

RegistrationWidget::RegistrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWidget)
{
    ui->setupUi(this);
    connect(ui->SignUpBtn, SIGNAL(clicked()), this, SLOT(OnSignUpBtnClicked()));
    connect(ui->MoveToLogInBtn, SIGNAL(clicked()), this, SLOT(OnMoveToLogInBtnClicked()));
}

RegistrationWidget::~RegistrationWidget()
{
    delete ui;
}

void RegistrationWidget::OnSignUpBtnClicked() {
    emit OnSignedUp();
}

void RegistrationWidget::OnMoveToLogInBtnClicked() {
    emit OnRegClicked(AuthorizationWindow::Transition::Authorization);
}
