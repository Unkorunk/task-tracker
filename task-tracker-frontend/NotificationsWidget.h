#ifndef NOTIFICATIONSWIDGET_H
#define NOTIFICATIONSWIDGET_H

#include "AbstractPage.h"

#include <QWidget>

namespace Ui {
class NotificationsWidget;
}

class NotificationsWidget : public AbstractPage
{
    Q_OBJECT

public:
    explicit NotificationsWidget(QWidget *parent = nullptr);
    ~NotificationsWidget();

protected:
    void SetupPage() override;

private:
    Ui::NotificationsWidget *ui;
};

#endif // NOTIFICATIONSWIDGET_H
