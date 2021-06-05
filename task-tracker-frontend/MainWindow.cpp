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

    connect(ui->navBar, &NavBar::NavButtonClicked, this, &MainWindow::OnTransition);
    connect(ui->navBar, &NavBar::ProfileButtonClicked, this, &MainWindow::OnProfileTransition);
    connect(ui->navBar, &NavBar::Logout, this, &MainWindow::OnLogout);

    connect(ui->navBar, &NavBar::BackButtonClicked, this, &MainWindow::OnBackButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartLoading()
{
    ui->loadingBar->StartLoading();
}

void MainWindow::StopLoading()
{
    ui->loadingBar->StopLoading();
}

void MainWindow::OnProjectTransition(const ProjectInfo &projectInfo)
{
    ProjectWidget* widget = dynamic_cast<ProjectWidget*>(ui->stackedWidget->widget(int(Transition::Project)));
    widget->SetupProject(projectInfo);
    OnTransition(Transition::Project);
}

void MainWindow::OnIssueTransition(const ProjectInfo& project, const TaskInfo &taskInfo)
{
    IssueWidget* widget = dynamic_cast<IssueWidget*>(ui->stackedWidget->widget(int(Transition::Issue)));
    widget->SetupTask(project, taskInfo);
    OnTransition(Transition::Issue);
}

void MainWindow::OnProfileTransition(const UserInfo& userInfo){
    ProfileWidget* widget = dynamic_cast<ProfileWidget*>(ui->stackedWidget->widget(int(Transition::Profile)));
    widget->SetupProfile(userInfo);
    OnTransition(Transition::Profile);
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
        StartLoading();
        break;
    }

    myTransitionsHistory.push((Transition)ui->stackedWidget->currentIndex());
    ui->stackedWidget->setCurrentIndex((int)transition);
}

void MainWindow::OnBackButtonClicked()
{
    if (myTransitionsHistory.empty()) {
        return;
    }

    Transition prevTransition = myTransitionsHistory.pop();
    OnTransition(prevTransition);
    myTransitionsHistory.pop();
}

void MainWindow::OnLogout()
{
    myAuthWindow.show();
    this->close();
}

