#include "NotificationsWidget.h"
#include "ui_NotificationsWidget.h"

NotificationsWidget::NotificationsWidget(QWidget *parent) :
    AbstractPage(parent),
    ui(new Ui::NotificationsWidget) {
    ui->setupUi(this);
}

NotificationsWidget::~NotificationsWidget() {
    delete ui;
}

void NotificationsWidget::SetupPage() { }
