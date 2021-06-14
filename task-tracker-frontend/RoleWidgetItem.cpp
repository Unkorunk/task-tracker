#include "Backend.h"
#include "MainWindow.h"
#include "RoleWidgetItem.h"
#include "ui_RoleWidgetItem.h"

RoleWidgetItem::RoleWidgetItem(QWidget *parent) :
        QWidget(parent), myRole(Context::DEFAULT_ROLE), myUserRole(Context::DEFAULT_ROLE),
        isEditable(false), ui(new Ui::RoleWidgetItem) {
    ui->setupUi(this);

    connect(ui->EditCancelBtn, &QAbstractButton::clicked, this, &RoleWidgetItem::OnEditCancelBtnClicked);
    connect(ui->SaveBtn, &QAbstractButton::clicked, this, &RoleWidgetItem::OnSaveBtnClicked);
    connect(ui->DeleteBtn, &QAbstractButton::clicked, this, &RoleWidgetItem::OnDeleteBtnClicked);
}

RoleWidgetItem::~RoleWidgetItem() {
    delete ui;
}

RoleInfo RoleWidgetItem::GetRole() const {
    myRole.SetCaption(ui->captionText->text());

    myRole.SetPermissions(RoleInfo::MANAGE_OWN_TASK, ui->ownTaskBox->isChecked());
    myRole.SetPermissions(RoleInfo::MANAGE_ALL_TASK, ui->allTaskBox->isChecked());
    myRole.SetPermissions(RoleInfo::MANAGE_OWN_COMMENT, ui->ownCommentBox->isChecked());
    myRole.SetPermissions(RoleInfo::MANAGE_ALL_COMMENT, ui->allCommentsBox->isChecked());
    myRole.SetPermissions(RoleInfo::MANAGE_TEAM, ui->teamBox->isChecked());
    myRole.SetPermissions(RoleInfo::MANAGE_ROLES, ui->rolesBox->isChecked());
    myRole.SetPermissions(RoleInfo::MANAGE_PROJECT, ui->editProjectBox->isChecked());
    myRole.SetPermissions(RoleInfo::DELETE_PROJECT, ui->deleteProjectBox->isChecked());

    return myRole;
}

void RoleWidgetItem::SetRole(const RoleInfo &role, const RoleInfo& userRole) {
    myRole = role;
    myUserRole = userRole;
    ReadonlyMode();

    ui->EditCancelBtn->hide();
    ui->DeleteBtn->hide();
    ui->SaveBtn->hide();
    if (userRole.HasPermission(RoleInfo::MANAGE_ROLES)) {
        ui->EditCancelBtn->show();
        ui->DeleteBtn->show();
        ui->SaveBtn->show();
    }

    ui->captionText->setText(role.GetCaption());

    ui->ownTaskBox->setChecked(role.HasPermission(RoleInfo::MANAGE_OWN_TASK));
    ui->allTaskBox->setChecked(role.HasPermission(RoleInfo::MANAGE_ALL_TASK));
    ui->ownCommentBox->setChecked(role.HasPermission(RoleInfo::MANAGE_OWN_COMMENT));
    ui->allCommentsBox->setChecked(role.HasPermission(RoleInfo::MANAGE_ALL_COMMENT));
    ui->teamBox->setChecked(role.HasPermission(RoleInfo::MANAGE_TEAM));
    ui->rolesBox->setChecked(role.HasPermission(RoleInfo::MANAGE_ROLES));
    ui->editProjectBox->setChecked(role.HasPermission(RoleInfo::MANAGE_PROJECT));
    ui->deleteProjectBox->setChecked(role.HasPermission(RoleInfo::DELETE_PROJECT));
}

void RoleWidgetItem::Clear() {
    ui->captionText->setText("");

    ui->ownTaskBox->setChecked(false);
    ui->allTaskBox->setChecked(false);
    ui->ownCommentBox->setChecked(false);
    ui->allCommentsBox->setChecked(false);
    ui->teamBox->setChecked(false);
    ui->rolesBox->setChecked(false);
    ui->editProjectBox->setChecked(false);
    ui->deleteProjectBox->setChecked(false);
}

void RoleWidgetItem::EditMode() {
    isEditable = true;

    ui->captionText->setEnabled(true);

    ui->ownTaskBox->setEnabled(true);
    ui->allTaskBox->setEnabled(true);
    ui->ownCommentBox->setEnabled(true);
    ui->allCommentsBox->setEnabled(true);
    ui->teamBox->setEnabled(true);
    ui->rolesBox->setEnabled(true);
    ui->editProjectBox->setEnabled(true);
    ui->deleteProjectBox->setEnabled(true);

    ui->SaveBtn->setEnabled(true);
    ui->EditCancelBtn->setText("Cancel");
}

void RoleWidgetItem::ReadonlyMode(){
    isEditable = false;

    ui->captionText->setEnabled(false);

    ui->ownTaskBox->setEnabled(false);
    ui->allTaskBox->setEnabled(false);
    ui->ownCommentBox->setEnabled(false);
    ui->allCommentsBox->setEnabled(false);
    ui->teamBox->setEnabled(false);
    ui->rolesBox->setEnabled(false);
    ui->editProjectBox->setEnabled(false);
    ui->deleteProjectBox->setEnabled(false);

    ui->SaveBtn->setEnabled(false);
    ui->EditCancelBtn->setText("Edit");
}

void RoleWidgetItem::OnEditCancelBtnClicked() {
    if (isEditable) {
        SetRole(myRole, myUserRole);
    } else {
        EditMode();
    }
}

void RoleWidgetItem::OnSaveBtnClicked() {
    RoleInfo newRole(myRole.GetId(), myRole.GetCaption(), 0, myRole.GetProjectId());

    newRole.SetCaption(ui->captionText->text());

    newRole.SetPermissions(RoleInfo::MANAGE_OWN_TASK, ui->ownTaskBox->isChecked());
    newRole.SetPermissions(RoleInfo::MANAGE_ALL_TASK, ui->allTaskBox->isChecked());
    newRole.SetPermissions(RoleInfo::MANAGE_OWN_COMMENT, ui->ownCommentBox->isChecked());
    newRole.SetPermissions(RoleInfo::MANAGE_ALL_COMMENT, ui->allCommentsBox->isChecked());
    newRole.SetPermissions(RoleInfo::MANAGE_TEAM, ui->teamBox->isChecked());
    newRole.SetPermissions(RoleInfo::MANAGE_ROLES, ui->rolesBox->isChecked());
    newRole.SetPermissions(RoleInfo::MANAGE_PROJECT, ui->editProjectBox->isChecked());
    newRole.SetPermissions(RoleInfo::DELETE_PROJECT, ui->deleteProjectBox->isChecked());

    Backend::Instance.EditRole(newRole);
}

void RoleWidgetItem::OnDeleteBtnClicked() {
    Backend::Instance.DeleteRole(myRole);
}

