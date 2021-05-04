#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"

#include <QDebug>

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::SetupProject(const ProjectInfo &project)
{
    qInfo() << project.projectName;
}
