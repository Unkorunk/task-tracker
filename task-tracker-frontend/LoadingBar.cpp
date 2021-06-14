#include "LoadingBar.h"
#include "ui_LoadingBar.h"
#include "QtWaitingSpinner/waitingspinnerwidget.h"
#include "Backend.h"
#include "AuthorizationWidget.h"

LoadingBar::LoadingBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingBar)
{
    ui->setupUi(this);
    ui->waitingSpinner->setLineLength(5);
    ui->waitingSpinner->setInnerRadius(5);
    ui->label->hide();

    connect(&Backend::Instance, &Backend::SignInFailed, this, &LoadingBar::FailLoading);
    //connect(, &AuthorizationWidget::FailAuthorization, this, &LoadingBar::FailLoading)
}

LoadingBar::~LoadingBar()
{
    delete ui;
}

void LoadingBar::StartLoading()
{
    ui->waitingSpinner->start();
    ui->label->setText("Waiting for response...");
    ui->label->show();
}

void LoadingBar::StopLoading()
{
    ui->waitingSpinner->stop();
    ui->label->hide();
}

void LoadingBar::FailLoading(QString log)
{
    ui->waitingSpinner->stop();
    //я бы принимал сюда текст снаружи
    ui->label->setText(log);
    ui->label->show();
}
