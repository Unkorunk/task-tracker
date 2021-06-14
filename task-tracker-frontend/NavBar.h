#ifndef NAVBAR_H
#define NAVBAR_H

#include <QGroupBox>
#include "MainWindow.h"

namespace Ui {
class NavBar;
}

class NavBar : public QGroupBox
{
    Q_OBJECT

public: 
    explicit NavBar(QWidget *parent = nullptr);
    ~NavBar();

signals:
    void NavButtonClicked(MainWindow::Transition transition);
    void ProfileButtonClicked(const UserInfo& user);
    void Logout();
    void BackButtonClicked();

private slots:
    void OnProjectBtnClicked();
    void OnNotificationBtnClicked();
    void OnProfileBtnClicked();
    void OnLogoutBtnClicked();
    void OnBackBtnClicked();

    void OnSignedIn(Status status);
    void OnInfoUpdateTime();
    void OnNotificationsLoaded(Status status, const QList<NotificationInfo>& notifications);
    void OnUnreadsLoaded(Status status, const QList<NotificationInfo>& notifications);
private:
    Ui::NavBar *ui;
    QTimer* myTimer;
};

#endif // NAVBAR_H
