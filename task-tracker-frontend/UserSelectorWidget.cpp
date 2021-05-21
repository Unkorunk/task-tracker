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
    ui->userSelector->addItem("None");
    ui->userSelector->setCurrentIndex(0);

    for (int i = 0; i < users.count(); i++) {
        ui->userSelector->addItem(users[i].GetFullName());
        if (selectedUser.has_value() && users[i].GetId() == selectedUser.value().GetId()) {
            ui->userSelector->setCurrentIndex(i + 1);
        }
    }
}

UserInfo UserSelectorWidget::GetData() const {
    return myUsers[ui->userSelector->currentIndex()];
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
