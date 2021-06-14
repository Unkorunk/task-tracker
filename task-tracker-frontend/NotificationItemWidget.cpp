#include "NotificationItemWidget.h"
#include "ui_NotificationItemWidget.h"

NotificationItemWidget::NotificationItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationItemWidget)
{
    ui->setupUi(this);

}

NotificationItemWidget::~NotificationItemWidget()
{
    delete ui;
}


void NotificationItemWidget::SetNotification(const NotificationInfo& notification) {
    ui->notificationText->setText(notification.GetText());
}
