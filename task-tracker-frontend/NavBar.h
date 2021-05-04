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
    void Logout();

private slots:
    void OnProjectBtnClicked();
    void OnNotificationBtnClicked();
    void OnProfileBtnClicked();
    void OnLogoutBtnClicked();

private:
    Ui::NavBar *ui;
};

#endif // NAVBAR_H
