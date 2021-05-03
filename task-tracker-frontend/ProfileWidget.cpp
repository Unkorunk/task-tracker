#include "ProfileWidget.h"
#include "ui_ProfileWidget.h"

ProfileWidget::ProfileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileWidget)
{
    ui->setupUi(this);
}

ProfileWidget::~ProfileWidget()
{
    delete ui;
}
