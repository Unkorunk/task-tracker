#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow* MainWindow::Instance = nullptr;

MainWindow::MainWindow(QMainWindow& authWindow, QWidget *parent)
    : QMainWindow(parent),
      myAuthWindow(authWindow),
      ui(new Ui::MainWindow)
{

    Instance = this;
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(4);

    connect(ui->navBar, &NavBar::NavButtonClicked, this, &MainWindow::OnTransition);
    connect(ui->navBar, &NavBar::Logout, this, &MainWindow::OnLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnProjectTransition(const ProjectInfo &projectInfo)
{
    ProjectWidget* widget = dynamic_cast<ProjectWidget*>(ui->stackedWidget->widget(int(Transition::Project)));
    widget->SetupProject(projectInfo);
    OnTransition(Transition::Project);
}

void MainWindow::OnIssueTransition(const TaskInfo &taskInfo)
{
    IssueWidget* widget = dynamic_cast<IssueWidget*>(ui->stackedWidget->widget(int(Transition::Issue)));
    widget->SetupTask(taskInfo);
    OnTransition(Transition::Issue);
}

void MainWindow::OnProjectSettingsTransition(const ProjectInfo &projectInfo)
{
    ProjectSettingsWidget* widget = dynamic_cast<ProjectSettingsWidget*>(ui->stackedWidget->widget(int(Transition::ProjectSettings)));
    widget->SetupProject(projectInfo);
    OnTransition(Transition::ProjectSettings);
}

void MainWindow::OnProjectStatisticsTransition(const ProjectInfo &projectInfo)
{
    StatisticsWidget* widget = dynamic_cast<StatisticsWidget*>(ui->stackedWidget->widget(int(Transition::ProjectSettings)));
    widget->SetupProject(projectInfo);
    OnTransition(Transition::ProjectSettings);
}

void MainWindow::OnTransition(MainWindow::Transition transition)
{
    switch (transition) {
    case Transition::Greetings:
        Backend::Instance.GetProjects();
        break;
    case Transition::Notifications:
    case Transition::Profile:
        break;
    }

    ui->stackedWidget->setCurrentIndex((int)transition);
}

void MainWindow::OnLogout()
{
    myAuthWindow.show();
    this->close();
}

