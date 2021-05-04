#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <QMainWindow>
#include "MainWindow.h"

namespace Ui {
class AuthorizationWindow;
}

class AuthorizationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);

    enum class Transition {
        Authorization = 0,
        Registration = 1,
    };


    ~AuthorizationWindow();

private slots:
    void OnTransition(AuthorizationWindow::Transition transition);
    void OnSwitchingWindow();

private:
    Ui::AuthorizationWindow *ui;
    MainWindow *mWindow;
};

#endif // AUTHORIZATIONWINDOW_H
