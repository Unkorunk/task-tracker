#include "ProjectSettingsWidget.h"
#include "ui_ProjectSettingsWidget.h"
#include "Backend.h"


ProjectSettingsWidget::ProjectSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSettingsWidget)
{
    ui->setupUi(this);

    //ui->label->setText(parent->objectName());

    connect( ui->saveNameButton, SIGNAL(clicked()), this, SLOT(OnSaveProjectName()) );

}

void ProjectSettingsWidget:: OnSaveProjectName(){
    //later need change ProjectInfo
    QString Obama = ui->editProjectName->text();
}

ProjectSettingsWidget::~ProjectSettingsWidget()
{
    delete ui;
}

void ProjectSettingsWidget::SetupProject(const ProjectInfo &project)
{
    ui->label->setText(project.projectName);
}
