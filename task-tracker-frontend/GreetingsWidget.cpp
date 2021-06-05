#include "GreetingsWidget.h"
#include "ui_GreetingsWidget.h"
#include "Backend.h"
#include "MainWindow.h"

GreetingsWidget::GreetingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GreetingsWidget)
{
    ui->setupUi(this);
    ui->personalProjectsList->ChangeHeader("Personal projects");

    connect(ui->projectList, &ProjectsList::AddProjectClicked, this, &GreetingsWidget::OnProjectAdd);
    connect(ui->projectList, &ProjectsList::ProjectSelected, MainWindow::Instance, &MainWindow::OnProjectTransition);

    connect(&Backend::Instance, &Backend::ProjectsLoaded, this, &GreetingsWidget::OnProjectsLoad);
    connect(&Backend::Instance, &Backend::ProfileUpdated, this, &GreetingsWidget::OnProfileUpdate);
}

GreetingsWidget::~GreetingsWidget()
{
    delete ui;
}

void GreetingsWidget::OnProjectsLoad(Status status, const QList<ProjectInfo> &projects)
{
    MainWindow::Instance->StopLoading();
    if (!status.isSuccess) {
        return;
    }

    ui->projectList->SetProjects(projects);
}

void GreetingsWidget::OnProjectAdd(const QString& name)
{
    MainWindow::Instance->StartLoading();
    Backend::Instance.CreateProject(name);
}

void GreetingsWidget::OnProfileUpdate(Status status, UserInfo user)
{
    ui->greetingsLabel->setText("Greetings, " + user.GetFullName() + "!");
}
