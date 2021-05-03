#include "NotificationsWidget.h"
#include "ui_NotificationsWidget.h"

NotificationsWidget::NotificationsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationsWidget)
{
    ui->setupUi(this);
}

NotificationsWidget::~NotificationsWidget()
{
    delete ui;
}
