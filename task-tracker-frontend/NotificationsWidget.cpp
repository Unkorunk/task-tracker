#include "NotificationsWidget.h"
#include "ui_NotificationsWidget.h"
#include "NotificationItemWidget.h"

NotificationsWidget::NotificationsWidget(QWidget *parent) :
    AbstractPage(parent),
    ui(new Ui::NotificationsWidget) {
    ui->setupUi(this);

    connect(&Backend::Instance, &Backend::UnreadLoaded, this, &NotificationsWidget::OnNotifictaionsLoaded);
}

NotificationsWidget::~NotificationsWidget() {
    delete ui;
}

void NotificationsWidget::SetupPage() {
    Backend::Instance.GetUnreadNotifications();
}

void NotificationsWidget::OnNotifictaionsLoaded(Status status, const QList<NotificationInfo> &notifications) {
    ui->listWidget->clear();
    for (auto& notification : notifications) {
        notificationsList.append(notification);
        auto item = new QListWidgetItem();
        auto widget = new NotificationItemWidget(this);
        widget->SetNotification(notification);
        item->setSizeHint(QSize(450, 60));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
        update();
    }
}

