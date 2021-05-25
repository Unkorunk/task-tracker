#include "ProjectSettingsWidget.h"
#include "ui_ProjectSettingsWidget.h"
#include "Backend.h"
#include "MainWindow.h"
#include "RoleWidgetItem.h"


ProjectSettingsWidget::ProjectSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSettingsWidget),
    myProject(-1, "Default project")
{
    ui->setupUi(this);

    ToReadonlyMode();

    connect(ui->saveNameButton, SIGNAL(clicked()), this, SLOT(OnSaveClicked()));
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnCancelClicked);

    connect(ui->createRole, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnRoleCreateClicked);

    connect(&Backend::Instance, &Backend::ProjectEdited, this, &ProjectSettingsWidget::OnProjectEdited);
    connect(&Backend::Instance, &Backend::RolesLoaded, this, &ProjectSettingsWidget::OnRolesLoaded);
    connect(&Backend::Instance, &Backend::RoleDeleted, this, &ProjectSettingsWidget::OnRoleDeleted);
}

void ProjectSettingsWidget::OnSaveClicked() {
    if (ui->cancelButton->isVisible()) {
        // TODO: Handle errors
        if (ui->editProjectName->text().count() == 0) {
            return;
        }

        MainWindow::Instance->StartLoading();
        ProjectInfo newProjectInfo(myProject.GetId(), ui->editProjectName->text());
        Backend::Instance.EditProject(newProjectInfo);
        LockUi();
    } else {
        ToEditMode();
    }
}

void ProjectSettingsWidget::OnCancelClicked() {
    ui->editProjectName->setText(myProject.GetTitle());
    ToReadonlyMode();
}

void ProjectSettingsWidget::OnProjectEdited(Status status)
{
    UnlockUi();
    MainWindow::Instance->StopLoading();
    if (status.isSuccess) {
       myProject.SetTitle(ui->editProjectName->text());
       ToReadonlyMode();
    } else {
        // TODO: Handle errors
    }

}

void ProjectSettingsWidget::OnRoleCreateClicked() {
    MainWindow::Instance->StartLoading();
    Backend::Instance.CreateRole(RoleInfo(-1, "User role", QByteArray(8, (char)0), myProject.GetId()));
}

void ProjectSettingsWidget::OnRolesLoaded(Status status, const QList<RoleInfo> &roles) {
    MainWindow::Instance->StopLoading();

    if (!status.isSuccess) {
        // TODO: Handle errors;
        return;
    }

    ui->rolesList->clear();
    for (auto& role : roles) {
       auto item = new QListWidgetItem();
       auto widget = new RoleWidgetItem(this);
       widget->SetRole(role);
       item->setSizeHint(QSize(800, 100));

       ui->rolesList->addItem(item);
       ui->rolesList->setItemWidget(item, widget);
    }
}

void ProjectSettingsWidget::OnRoleCreated(Status status, const RoleInfo &role) {
    MainWindow::Instance->StopLoading();

    if (!status.isSuccess) {
        // TODO: Handle errors
        return;
    }

    auto item = new QListWidgetItem();
    auto widget = new RoleWidgetItem(this);
    widget->SetRole(role);
    item->setSizeHint(QSize(800, 100));

    ui->rolesList->addItem(item);
    ui->rolesList->setItemWidget(item, widget);
}

void ProjectSettingsWidget::OnRoleEdited(Status status, const RoleInfo &role) {
     MainWindow::Instance->StopLoading();

     if (!status.isSuccess) {
         // TODO: Handle errors
         return;
     }

     for (int i = 0; i < ui->rolesList->count(); i++) {
         RoleWidgetItem* item = (RoleWidgetItem*)ui->rolesList->itemWidget(ui->rolesList->item(i));
         if (item->GetRole().GetId() == role.GetId()) {
             item->SetRole(role);
             return;
         }
     }
}

void ProjectSettingsWidget::OnRoleDeleted(Status status) {
    MainWindow::Instance->StopLoading();

    if (!status.isSuccess) {
        // TODO: Handle errors;
        return;
    }

    MainWindow::Instance->StartLoading();
    Backend::Instance.GetRoles(myProject);
}

ProjectSettingsWidget::~ProjectSettingsWidget() {
    delete ui;
}

void ProjectSettingsWidget::SetupProject(const ProjectInfo &project) {
    myProject = project;
    ui->editProjectName->setText(project.GetTitle());
    ui->rolesList->clear();

    Backend::Instance.GetRoles(project);
    MainWindow::Instance->StartLoading();

    UnlockUi();
    ToReadonlyMode();
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
