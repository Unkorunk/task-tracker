#include "GreetingsWidget.h"
#include "ui_GreetingsWidget.h"
#include "backend.h"

GreetingsWidget::GreetingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GreetingsWidget)
{
    ui->setupUi(this);
    ui->personalProjectsList->ChangeHeader("Personal projects");

    connect(&Backend::Instance, &Backend::ProjectsLoaded, this, &GreetingsWidget::OnProjectsLoad);
    connect(ui->projectList, &ProjectsList::AddProjectClicked, this, &GreetingsWidget::OnProjectAdd);
}

GreetingsWidget::~GreetingsWidget()
{
    delete ui;
}

void GreetingsWidget::OnProjectsLoad(Status status, const QList<ProjectInfo> &projects)
{
    if (!status.isSuccess) {
        return;
    }

    ui->projectList->SetProjects(projects);
}

void GreetingsWidget::OnProjectAdd(const QString& name)
{
    Backend::Instance.CreateProject(name);
}
