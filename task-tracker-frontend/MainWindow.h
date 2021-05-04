#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* Instance;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum class Transition {
        Greetings = 0,
        Profile = 1,
        Notifications = 2,
        Project = 3,
    };

public slots:
    void OnProjectTransition(const ProjectInfo& projectInfo);

private slots:
    void OnSayHello();
    void OnTransition(MainWindow::Transition transition);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
