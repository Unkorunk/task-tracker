#include "ProjectSettingsWidget.h"
#include "ui_ProjectSettingsWidget.h"
#include "Backend.h"


ProjectSettingsWidget::ProjectSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSettingsWidget),
    myProject(0, 0, "Default project")
{
    ui->setupUi(this);

    ToReadonlyMode();

    connect( ui->saveNameButton, SIGNAL(clicked()), this, SLOT(OnSaveClicked()) );
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnCancelClicked);

    connect(&Backend::Instance, &Backend::ProjectEdited, this, &ProjectSettingsWidget::OnProjectEdited);
}

void ProjectSettingsWidget::OnSaveClicked() {
    if (ui->cancelButton->isVisible()) {
        // TODO: Handle errors
        if (ui->editProjectName->text().count() == 0) {
            return;
        }

        ProjectInfo newProjectInfo(myProject.id, myProject.projectId, ui->editProjectName->text());
        Backend::Instance.EditProject(newProjectInfo);
        LockUi();
    } else {
        ToEditMode();
    }

}

void ProjectSettingsWidget::OnCancelClicked() {
    ui->editProjectName->setText(myProject.projectName);
    ToReadonlyMode();
}

void ProjectSettingsWidget::OnProjectEdited(Status status)
{
    UnlockUi();
    if (status.isSuccess) {
       myProject.projectName = ui->editProjectName->text();
       ToReadonlyMode();
    } else {
        // TODO: Handle errors
    }

}

ProjectSettingsWidget::~ProjectSettingsWidget()
{
    delete ui;
}

void ProjectSettingsWidget::SetupProject(const ProjectInfo &project)
{
    myProject = project;
    ui->editProjectName->setText(project.projectName);
}

void ProjectSettingsWidget::ToEditMode()
{
    ui->editProjectName->setStyleSheet("* { background-color: rgba(255, 255, 255, 255); }");
    ui->editProjectName->setReadOnly(false);

    ui->cancelButton->setVisible(true);
    ui->saveNameButton->setText("Save");
}

void ProjectSettingsWidget::ToReadonlyMode()
{
    ui->editProjectName->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");
    ui->editProjectName->setReadOnly(true);

    ui->cancelButton->setVisible(false);
    ui->saveNameButton->setText("Edit");
}

void ProjectSettingsWidget::LockUi()
{
    ui->editProjectName->setReadOnly(true);
    ui->saveNameButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);
}

void ProjectSettingsWidget::UnlockUi()
{
    ui->editProjectName->setReadOnly(false);
    ui->saveNameButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);
}
