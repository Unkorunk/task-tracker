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
    static MainWindow* Instance;

    MainWindow(QMainWindow& authWindow, QWidget *parent = nullptr);
    ~MainWindow();

    void StartLoading();

    void StopLoading();

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

    void OnIssueTransition(const ProjectInfo& project, const TaskInfo& taskInfo);

    void OnProjectSettingsTransition(const ProjectInfo& projectInfo);

    void OnProjectStatisticsTransition(const ProjectInfo& projectInfo);

private slots:
    void OnLogout();

    void OnTransition(MainWindow::Transition transition);

    void OnBackButtonClicked();

private:
    Ui::MainWindow *ui;

    QMainWindow& myAuthWindow;

    QStack<MainWindow::Transition> myTransitionsHistory;
};
#endif // MAINWINDOW_H
