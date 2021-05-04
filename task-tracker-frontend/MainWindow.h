#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* Instance;

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

public slots:
    void OnProjectTransition(const ProjectInfo& projectInfo);

    void OnIssueTransition(const TaskInfo& taskInfo);

    void OnProjectSettingsTransition(const ProjectInfo& projectInfo);

    void OnProjectStatisticsTransition(const ProjectInfo& projectInfo);

private slots:
    void OnTransition(MainWindow::Transition transition);
    void OnLogout();

private:
    Ui::MainWindow *ui;

    QMainWindow& myAuthWindow;
};
#endif // MAINWINDOW_H
