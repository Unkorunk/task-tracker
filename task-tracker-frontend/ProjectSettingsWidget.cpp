#include "ProjectSettingsWidget.h"
#include "ui_ProjectSettingsWidget.h"
#include "Backend.h"
#include "MainWindow.h"
#include "RoleWidgetItem.h"
#include "UserItemWidget.h"


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
    connect(&Backend::Instance, &Backend::RoleCreated, this, &ProjectSettingsWidget::OnRoleCreated);
    connect(&Backend::Instance, &Backend::RoleEdited, this, &ProjectSettingsWidget::OnRoleEdited);

    connect(ui->inviteButton, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnInviteClicked);
    connect(&Backend::Instance, &Backend::MemberInvited, this, &ProjectSettingsWidget::OnMemberInvited);
    connect(&Backend::Instance, &Backend::MemberKicked, this, &ProjectSettingsWidget::OnMemberKicked);
    connect(&Backend::Instance, &Backend::ProjectUsersLoaded, this, &ProjectSettingsWidget::OnUsersLoaded);
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
    Backend::Instance.CreateRole(RoleInfo(-1, "User role", 0, myProject.GetId()));
}

void ProjectSettingsWidget::OnRolesLoaded(Status status, const QList<RoleInfo> &roles) {
    MainWindow::Instance->StopLoading();

    if (!status.isSuccess) {
        // TODO: Handle errors;
        return;
    }

    myRoles = roles;
    ui->rolesList->clear();
    ui->roleSelector->clear();

    for (auto& role : roles) {
       auto item = new QListWidgetItem();
       auto widget = new RoleWidgetItem(this);
       widget->SetRole(role);
       item->setSizeHint(QSize(800, 100));

       ui->rolesList->addItem(item);
       ui->rolesList->setItemWidget(item, widget);

       ui->roleSelector->addItem(role.GetCaption());
    }

    Backend::Instance.GetProjectUsers(myProject);
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
    ui->roleSelector->addItem(role.GetCaption());
    myRoles.append(role);
    Backend::Instance.GetProjectUsers(myProject);
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

    Backend::Instance.GetProjectUsers(myProject);
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

void ProjectSettingsWidget::OnInviteClicked() {
    MainWindow::Instance->StartLoading();
    Backend::Instance.InviteByEmail(myProject, myRoles[ui->roleSelector->currentIndex()], ui->EmailOfInvitedUser->text());
}

void ProjectSettingsWidget::OnMemberInvited(Status status){
    MainWindow::Instance->StopLoading();
    if (!status.isSuccess) {
        // TODO: handle errors
        return;
    }

    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnMemberKicked(Status status) {
    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnUsersLoaded(Status status, const QList<QPair<UserInfo, RoleInfo> > &users) {
    if (!status.isSuccess) {
        // TODO: Handle errors;
        return;
    }

    ui->teamList->clear();
    for (auto& entry : users) {
        auto item = new QListWidgetItem();
        auto widget = new UserItemWidget(this);
        widget->SetRoles(entry.first, myRoles, myProject, entry.second.GetId());
        item->setSizeHint(QSize(800, 50));

        ui->teamList->addItem(item);
        ui->teamList->setItemWidget(item, widget);
    }
}

ProjectSettingsWidget::~ProjectSettingsWidget() {
    delete ui;
}

void ProjectSettingsWidget::SetupProject(const ProjectInfo &project) {
    myProject = project;
    ui->editProjectName->setText(project.GetTitle());
    ui->rolesList->clear();
    ui->teamList->clear();

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
