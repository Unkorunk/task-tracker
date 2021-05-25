#include "Backend.h"
#include "MainWindow.h"
#include "UserItemWidget.h"
#include "ui_UserItemWidget.h"

UserItemWidget::UserItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItemWidget),
    myUser(-1, "", "", ""),
    myProject(-1, "") {
    ui->setupUi(this);
}

UserItemWidget::~UserItemWidget() {
    delete ui;
}

void UserItemWidget::SetRoles(const UserInfo &user, const QList<RoleInfo> &roles, const ProjectInfo& projectInfo, int roleId) {
    myRoles = roles;
    myUser = user;
    myProject = projectInfo;

    ui->userLabel->setText(user.GetFullName());

    int currentRole = 0;
    for (int i = 0; i < roles.size(); i++) {
        ui->roleSelector->addItem(roles[i].GetCaption());
        if (roles[i].GetId() == roleId) {
            currentRole = i;
        }
    }

    ui->roleSelector->setCurrentIndex(currentRole);

    connect(ui->roleSelector, &QComboBox::currentIndexChanged, this, &UserItemWidget::OnRoleChangeRequested);
    connect(ui->kickBtn, &QAbstractButton::clicked, this, &UserItemWidget::OnKickClicked);
    connect(&Backend::Instance, &Backend::RoleChanged, this, &UserItemWidget::OnRoleChanged);
}

void UserItemWidget::OnRoleChangeRequested() {
    MainWindow::Instance->StartLoading();
    Backend::Instance.ChangeRole(myUser, myRoles[ui->roleSelector->currentIndex()]);
}

void UserItemWidget::OnRoleChanged(Status status) {
    MainWindow::Instance->StopLoading();
}

void UserItemWidget::OnKickClicked() {
    Backend::Instance.Kick(myProject, myUser);
}
