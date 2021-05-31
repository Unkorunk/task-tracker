#include "GreetingsWidget.h"
#include "ui_GreetingsWidget.h"
#include "Backend.h"
#include "MainWindow.h"

GreetingsWidget::GreetingsWidget(QWidget *parent) : AbstractPage(parent),
        ui(new Ui::GreetingsWidget) {
    ui->setupUi(this);

    ui->personalProjectsList->ChangeHeader("Personal projects");
    SetupPage();

    ui->personalProjectsList->setStyleSheet("ProjectsList {background-color: white; border-radius: 20px;}");
    ui->projectList->setStyleSheet("background-color: rgb(232, 227, 227);");


    connect(ui->projectList, &ProjectsList::AddProjectClicked, this, &GreetingsWidget::OnProjectAdd);
    connect(ui->projectList, &ProjectsList::ProjectSelected, this, &GreetingsWidget::OnProjectSelected);

    connect(&Backend::Instance, &Backend::ProjectsLoaded, this, &GreetingsWidget::OnProjectsLoad);
    connect(&Backend::Instance, &Backend::ProfileUpdated, this, &GreetingsWidget::OnProfileUpdate);
}

GreetingsWidget::~GreetingsWidget() {
    delete ui;
}

void GreetingsWidget::SetupPage() { }

void GreetingsWidget::OnProjectSelected(const ProjectInfo &project, const RoleInfo &role) {
    myContext.SetProject(project);
    myContext.SetRole(role);
    emit ProjectSelected(MainWindow::Transition::Project, myContext);
}

void GreetingsWidget::OnProjectsLoad(Status status, const QList<QPair<ProjectInfo, RoleInfo>> &projects) {
    if (!status.isSuccess) {
        return;
    }

    ui->projectList->SetProjects(projects);
}

void GreetingsWidget::OnProjectAdd(const QString& name) {
    Backend::Instance.CreateProject(name);
}



void GreetingsWidget::OnProfileUpdate(Status status, const UserInfo& user) {
    myContext.SetUser(user);
    ui->greetingsLabel->setText("Welcome, " + user.GetFullName() + "!");
}
