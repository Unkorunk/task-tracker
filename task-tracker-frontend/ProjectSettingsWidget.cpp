#include "ProjectSettingsWidget.h"
#include "ui_ProjectSettingsWidget.h"
#include "Backend.h"
#include "MainWindow.h"
#include "RoleWidgetItem.h"
#include "UserItemWidget.h"
#include "PropertyItemWidget.h"


ProjectSettingsWidget::ProjectSettingsWidget(QWidget *parent) :
        AbstractPage(parent), ui(new Ui::ProjectSettingsWidget), myProject(-1, "Default project") {
    ui->setupUi(this);

    ToReadonlyMode();

    connect(ui->saveNameButton, SIGNAL(clicked()), this, SLOT(OnSaveClicked()));
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnCancelClicked);
    connect(ui->createRole, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnRoleCreateClicked);
    connect(ui->inviteButton, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnInviteClicked);
    connect(ui->newTagBtn, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnTagCreateClicked);
    connect(ui->deleteBtn, &QAbstractButton::clicked, this, &ProjectSettingsWidget::OnDeleteClicked);

    connect(&Backend::Instance, &Backend::ProjectDeleted, this, &ProjectSettingsWidget::OnProjectDeleted);

    connect(&Backend::Instance, &Backend::ProjectEdited, this, &ProjectSettingsWidget::OnProjectEdited);
    connect(&Backend::Instance, &Backend::RolesLoaded, this, &ProjectSettingsWidget::OnRolesLoaded);
    connect(&Backend::Instance, &Backend::RoleDeleted, this, &ProjectSettingsWidget::OnRoleDeleted);
    connect(&Backend::Instance, &Backend::RoleCreated, this, &ProjectSettingsWidget::OnRoleCreated);
    connect(&Backend::Instance, &Backend::RoleEdited, this, &ProjectSettingsWidget::OnRoleEdited);
    connect(&Backend::Instance, &Backend::MemberInvited, this, &ProjectSettingsWidget::OnMemberInvited);
    connect(&Backend::Instance, &Backend::MemberKicked, this, &ProjectSettingsWidget::OnMemberKicked);
    connect(&Backend::Instance, &Backend::ProjectUsersLoaded, this, &ProjectSettingsWidget::OnUsersLoaded);

    connect(&Backend::Instance, &Backend::TagCaptionsLoaded, this, &ProjectSettingsWidget::OnTagsLoaded);
    connect(&Backend::Instance, &Backend::TagCaptionDeleted, this, &ProjectSettingsWidget::OnTagDeleted);
    connect(&Backend::Instance, &Backend::TagCaptionCreated, this, &ProjectSettingsWidget::OnTagCreated);
    connect(&Backend::Instance, &Backend::TagCaptionEdited, this, &ProjectSettingsWidget::OnTagEdited);

    ui->deleteBtn->hide();
}

void ProjectSettingsWidget::OnSaveClicked() {
    if (ui->cancelButton->isVisible()) {
        if (ui->editProjectName->text().count() == 0) {
            emit Backend::Instance.RequestFailed("У проекта должно быть название!");
            return;
        }

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

void ProjectSettingsWidget::OnProjectEdited(Status status) {
    UnlockUi();
    if (status.isSuccess) {
       myProject.SetTitle(ui->editProjectName->text());
       ToReadonlyMode();
    } else {
        emit Backend::Instance.RequestFailed("Не удалось сохранить изменения!");
    }
}

void ProjectSettingsWidget::OnDeleteClicked() {
    Backend::Instance.DeleteProject(myContext.GetCurrentProject());
}

void ProjectSettingsWidget::OnProjectDeleted(Status status) {
    if (status.isSuccess) {
        myContext.SetProject(Context::DEFAULT_PROJECT);
        emit TransitionRequested(MainWindow::Transition::Greetings, myContext);
    }
}

void ProjectSettingsWidget::OnRoleCreateClicked() {
    Backend::Instance.CreateRole(RoleInfo(-1, "User role", 0, myProject.GetId()));
}

void ProjectSettingsWidget::OnRolesLoaded(Status status, const QList<RoleInfo> &roles) {
    if (!status.isSuccess) {
        return;
    }

    myRoles = roles;
    ui->rolesList->clear();
    ui->roleSelector->clear();

    for (auto& role : roles) {
       auto item = new QListWidgetItem();
       auto widget = new RoleWidgetItem(this);
       widget->SetRole(role, myContext.GetCurrentRole());
       item->setSizeHint(QSize(800, 100));

       ui->rolesList->addItem(item);
       ui->rolesList->setItemWidget(item, widget);

       ui->roleSelector->addItem(role.GetCaption());
    }

    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnRoleCreated(Status status, const RoleInfo &role) {
    if (!status.isSuccess) {
        return;
    }

    auto item = new QListWidgetItem();
    auto widget = new RoleWidgetItem(this);
    widget->SetRole(role, myContext.GetCurrentRole());
    item->setSizeHint(QSize(800, 100));

    ui->rolesList->addItem(item);
    ui->rolesList->setItemWidget(item, widget);
    ui->roleSelector->addItem(role.GetCaption());
    myRoles.append(role);
    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnRoleEdited(Status status, const RoleInfo &role) {
    if (!status.isSuccess) {
        return;
    }

    for (int i = 0; i < ui->rolesList->count(); i++) {
        RoleWidgetItem* item = (RoleWidgetItem*)ui->rolesList->itemWidget(ui->rolesList->item(i));
        if (item->GetRole().GetId() == role.GetId()) {
            item->SetRole(role, myContext.GetCurrentRole());
            return;
        }
    }

    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnRoleDeleted(Status status) {
    if (!status.isSuccess) {
        return;
    }

    Backend::Instance.GetRoles(myProject);
}

void ProjectSettingsWidget::OnInviteClicked() {
    Backend::Instance.InviteByEmail(myProject, myRoles[ui->roleSelector->currentIndex()], ui->EmailOfInvitedUser->text());
}

void ProjectSettingsWidget::OnMemberInvited(Status status){
    if (!status.isSuccess) {
        return;
    }

    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnMemberKicked(Status status) {
    if (!status.isSuccess) {
        return;
    }

    Backend::Instance.GetProjectUsers(myProject);
}

void ProjectSettingsWidget::OnUsersLoaded(Status status, const QList<QPair<UserInfo, RoleInfo> > &users) {
    if (!status.isSuccess) {
        return;
    }

    ui->teamList->clear();
    for (auto& entry : users) {
        auto item = new QListWidgetItem();
        auto widget = new UserItemWidget(this);
        widget->SetRoles(entry.first, myRoles, myProject, entry.second.GetId(), myContext.GetCurrentRole().HasPermission(RoleInfo::MANAGE_TEAM));
        item->setSizeHint(QSize(800, 50));

        ui->teamList->addItem(item);
        ui->teamList->setItemWidget(item, widget);
    }
}

void ProjectSettingsWidget::OnTagCreateClicked() {
    Backend::Instance.CreateTagCaption(myContext.GetCurrentProject(), ui->propertyNameEdit->text());
}

void ProjectSettingsWidget::OnTagsLoaded(Status status, const QList<TagInfo> &tags) {
    if (!status.isSuccess) {
        return;
    }

    ui->tagsList->clear();

    for (auto& tag : tags) {
        auto item = new QListWidgetItem();
        auto widget = new PropertyItemWidget(this);
        widget->SetupTag(tag, myContext.GetCurrentRole());
        item->setSizeHint(QSize(200, 200));
        ui->tagsList->addItem(item);
        ui->tagsList->setItemWidget(item, widget);
    }
}

void ProjectSettingsWidget::OnTagDeleted(Status status) {
    if (!status.isSuccess) {
        return;
    }
    Backend::Instance.GetTagCaptions(myContext.GetCurrentProject());
}

void ProjectSettingsWidget::OnTagCreated(Status status, const TagInfo &tag){
    if (!status.isSuccess) {
        return;
    }

    auto item = new QListWidgetItem();
    auto widget = new PropertyItemWidget(this);
    widget->SetupTag(tag, myContext.GetCurrentRole());
    item->setSizeHint(QSize(200, 200));
    ui->tagsList->addItem(item);
    ui->tagsList->setItemWidget(item, widget);
}

void ProjectSettingsWidget::OnTagEdited(Status status, const TagInfo &tag) {
    if (!status.isSuccess) {
        return;
    }

    for (int i = 0; i < ui->tagsList->count(); i++) {
        PropertyItemWidget* it = (PropertyItemWidget*)ui->tagsList->itemWidget(ui->tagsList->item(i));
        if (it->GetTag().GetId() == tag.GetId()) {
            it->SetupTag(tag, myContext.GetCurrentRole());
            return;
        }
    }
}

ProjectSettingsWidget::~ProjectSettingsWidget() {
    delete ui;
}

void ProjectSettingsWidget::SetupPage() {
    myProject = myContext.GetCurrentProject();
    ui->cancelButton->hide();
    ui->createRole->hide();
    ui->inviteButton->hide();
    ui->newTagBtn->hide();
    ui->saveNameButton->hide();
    ui->deleteBtn->hide();

    ui->editProjectName->setText(myContext.GetCurrentProject().GetTitle());
    ui->rolesList->clear();
    ui->teamList->clear();
    ui->tagsList->clear();
    ui->tabWidget->setCurrentIndex(0);

    Backend::Instance.GetRoles(myContext.GetCurrentProject());
    Backend::Instance.GetTagCaptions(myContext.GetCurrentProject());

    RoleInfo role = myContext.GetCurrentRole();
    if (role.HasPermission(RoleInfo::MANAGE_PROJECT)) {
        ui->cancelButton->show();
        ui->saveNameButton->show();
        ui->newTagBtn->show();
    }
    if (role.HasPermission(RoleInfo::MANAGE_ROLES)) {
        ui->createRole->show();
    }
    if (role.HasPermission(RoleInfo::MANAGE_TEAM)) {
        ui->inviteButton->show();
    }
    if (role.HasPermission(RoleInfo::DELETE_PROJECT)) {
        //ui->deleteBtn->show();
    }

    UnlockUi();
    ToReadonlyMode();
}

void ProjectSettingsWidget::ToEditMode() {
    ui->editProjectName->setStyleSheet("* { background-color: rgba(255, 255, 255, 255); }");
    ui->editProjectName->setReadOnly(false);

    ui->cancelButton->setVisible(true);
    ui->saveNameButton->setText("Save");
}

void ProjectSettingsWidget::ToReadonlyMode() {
    ui->editProjectName->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");
    ui->editProjectName->setReadOnly(true);

    ui->cancelButton->setVisible(false);
    ui->saveNameButton->setText("Edit");
}

void ProjectSettingsWidget::LockUi() {
    ui->editProjectName->setReadOnly(true);
    ui->saveNameButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);
}

void ProjectSettingsWidget::UnlockUi() {
    ui->editProjectName->setReadOnly(false);
    ui->saveNameButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);
}
