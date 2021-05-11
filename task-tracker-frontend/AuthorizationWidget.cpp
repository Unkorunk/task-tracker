#include "AuthorizationWidget.h"
#include "ui_AuthorizationWidget.h"

AuthorizationWidget::AuthorizationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizationWidget)
{
    ui->setupUi(this);
    connect(ui->LogInBtn, SIGNAL(clicked()), this, SLOT(OnLogInBtnClicked()));
    connect(ui->MoveToSignUpBtn, SIGNAL(clicked()), this, SLOT(OnMoveToSignUpBtnClicked()));
}

AuthorizationWidget::~AuthorizationWidget()
{
    delete ui;
}

void AuthorizationWidget::OnLogInBtnClicked() {
    emit OnLoggedIn();
}

void AuthorizationWidget::OnMoveToSignUpBtnClicked() {
    emit OnAuthClicked(AuthorizationWindow::Transition::Registration);
}

