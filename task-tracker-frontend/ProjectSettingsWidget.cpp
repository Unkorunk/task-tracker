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

void ProjectSettingsWidget:: OnSaveProjectName(const ProjectInfo &project){
    //later need change ProjectInfo
    ProjectInfo new_project_info(project.id, project.projectId, ui->editProjectName->text());
    Backend::Instance.UpdateProject(new_project_info);
}

ProjectSettingsWidget::~ProjectSettingsWidget()
{
    delete ui;
}

void ProjectSettingsWidget::SetupProject(const ProjectInfo &project)
{
    ui->label->setText(project.projectName);
}
