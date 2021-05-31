#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include "Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow& authWindow, QWidget *parent = nullptr);
    ~MainWindow();

    enum class Transition {
        Greetings = 0,
        Profile = 1,
        Notifications = 2,
        Project = 3,
        ProjectSettings = 4,
        ProjectStatistics = 5,
        Issue = 6,
    };

private slots:
    void OnLogout();

    void OnNavBarTransition(MainWindow::Transition transition);

    void OnTransition(MainWindow::Transition transition, const Context& context);

    void OnBackButtonClicked();

    void OnLoadingChanged(bool isLoading);

private:
    Ui::MainWindow *ui;

    QMainWindow& myAuthWindow;

    Context myContext;

    QStack<QPair<MainWindow::Transition, Context>> myTransitionsHistory;
};
#endif // MAINWINDOW_H
