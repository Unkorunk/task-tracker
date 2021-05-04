#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow* MainWindow::Instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Instance = this;
    ui->setupUi(this);

    connect(ui->navBar, &NavBar::OnNavbarClicked, this, &MainWindow::OnTransition);

    OnTransition(Transition::Greetings);
    //connect( ui->pushButton, SIGNAL(clicked()), this, SLOT(OnSayHello()) );
}

void MainWindow::OnSayHello() {
    //ui->label->setText("Hello World");
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

