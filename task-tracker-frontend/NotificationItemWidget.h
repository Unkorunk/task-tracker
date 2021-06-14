#ifndef NOTIFICATIONITEMWIDGET_H
#define NOTIFICATIONITEMWIDGET_H

#include <QWidget>

#include "DataClasses.h"

namespace Ui {
class NotificationItemWidget;
}

class NotificationItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationItemWidget(QWidget *parent = nullptr);
    ~NotificationItemWidget();

    void SetNotification(const NotificationInfo& notification);

private:
    Ui::NotificationItemWidget *ui;

};

#endif // NOTIFICATIONITEMWIDGET_H
