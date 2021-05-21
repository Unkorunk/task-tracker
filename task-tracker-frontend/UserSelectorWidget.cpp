#include "UserSelectorWidget.h"
#include "ui_UserSelectorWidget.h"

UserSelectorWidget::UserSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSelectorWidget) {
    ui->setupUi(this);
    SetEditable(false);
}

UserSelectorWidget::~UserSelectorWidget() {
    delete ui;
}

void UserSelectorWidget::SetData(const QString &caption, const std::optional<UserInfo>& selectedUser, const QList<UserInfo>& users) {
    ui->captionLabel->setText(caption);
    myUsers = QList<UserInfo>();
    myUsers.append(UserInfo(-1, "None", "None", "None"));
    ui->userSelector->clear();
    ui->userSelector->addItem("None");
    ui->userSelector->setCurrentIndex(0);

    for (auto& user : users) {
        myUsers.append(user);
        ui->userSelector->addItem(user.GetFullName());
    }

    ChangeData(selectedUser);
}

void UserSelectorWidget::ChangeData(const std::optional<UserInfo> &selectedUser) {
    ui->userSelector->setCurrentIndex(0);

    for (int i = 0; i < myUsers.count(); i++) {
        if (selectedUser.has_value() && myUsers[i].GetId() == selectedUser.value().GetId()) {
            ui->userSelector->setCurrentIndex(i);
        }
    }
}

std::optional<UserInfo> UserSelectorWidget::GetData() const {
    std::optional<UserInfo> user;
    if (ui->userSelector->currentIndex() != 0) {
        user.emplace(myUsers[ui->userSelector->currentIndex()]);
    }

    return user;
}

void UserSelectorWidget::SetEditable(bool editable) {
    isEditable = editable;
    ui->userSelector->setEnabled(editable);
}

bool UserSelectorWidget::GetEditable() const {
    return isEditable;
}

bool UserSelectorWidget::IsEmpty() const {
    return myUsers.count() <= 1;
}
