#ifndef NOTIFICATIONSWIDGET_H
#define NOTIFICATIONSWIDGET_H

#include "AbstractPage.h"
#include "MainWindow.h"
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

signals:
    void TransitionRequested(MainWindow::Transition transition, const Context& context);

private slots:
    void OnNotifictaionsLoaded(Status status, const QList<NotificationInfo> &notificationsList);

private:
    Ui::NotificationsWidget *ui;

    QList<NotificationInfo> notificationsList;
};

#endif // NOTIFICATIONSWIDGET_H
