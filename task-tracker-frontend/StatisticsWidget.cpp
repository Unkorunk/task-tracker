#include "StatisticsWidget.h"
#include "ui_StatisticsWidget.h"
#include "Backend.h"

StatisticsWidget::StatisticsWidget(QWidget *parent) :
    AbstractPage(parent),
    ui(new Ui::StatisticsWidget) {
    ui->setupUi(this);
}

StatisticsWidget::~StatisticsWidget() {
    delete ui;
}

void StatisticsWidget::SetupPage() {
    ui->label->setText(myContext.GetCurrentProject().GetTitle());
}
