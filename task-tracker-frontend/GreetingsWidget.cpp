#include "GreetingsWidget.h"
#include "ui_GreetingsWidget.h"

GreetingsWidget::GreetingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GreetingsWidget)
{
    ui->setupUi(this);
}

GreetingsWidget::~GreetingsWidget()
{
    delete ui;
}
