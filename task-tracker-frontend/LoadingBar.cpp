#include "LoadingBar.h"
#include "ui_LoadingBar.h"
#include "QtWaitingSpinner/waitingspinnerwidget.h"

LoadingBar::LoadingBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingBar)
{
    ui->setupUi(this);
    ui->waitingSpinner->setLineLength(5);
    ui->waitingSpinner->setInnerRadius(5);
    ui->label->hide();
}

LoadingBar::~LoadingBar()
{
    delete ui;
}

void LoadingBar::StartLoading()
{
    ui->waitingSpinner->start();
    ui->label->show();
}

void LoadingBar::StopLoading()
{
    ui->waitingSpinner->stop();
    ui->label->hide();
}
