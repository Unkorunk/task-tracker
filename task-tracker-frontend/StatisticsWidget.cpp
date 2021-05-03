#include "StatisticsWidget.h"
#include "ui_StatisticsWidget.h"

StatisticsWidget::StatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}
