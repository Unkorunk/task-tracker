#include "StatisticsWidget.h"
#include "ui_StatisticsWidget.h"
#include "backend.h"

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

void StatisticsWidget::SetupProject(const ProjectInfo &project)
{
    ui->label->setText(project.projectName);
}
