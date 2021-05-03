#include "ProjectSettingsWidget.h"
#include "ui_ProjectSettingsWidget.h"

ProjectSettingsWidget::ProjectSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSettingsWidget)
{
    ui->setupUi(this);
}

ProjectSettingsWidget::~ProjectSettingsWidget()
{
    delete ui;
}
