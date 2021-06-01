#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QMainWindow& authWindow, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), myAuthWindow(authWindow) {
    ui->setupUi(this);


    ui->background->setStyleSheet("QWidget#background {background-color: rgb(232, 227, 227);border-radius: 20px;}");

    connect(ui->navBar, &NavBar::NavButtonClicked, this, &MainWindow::OnNavBarTransition);

    connect(ui->navBar, &NavBar::Logout, this, &MainWindow::OnLogout);
    connect(ui->navBar, &NavBar::BackButtonClicked, this, &MainWindow::OnBackButtonClicked);

    connect(ui->greetingsPage, &GreetingsWidget::ProjectSelected, this, &MainWindow::OnTransition);
    connect(ui->projectPage, &ProjectWidget::TransitionRequested, this, &MainWindow::OnTransition);
    connect(ui->issuePage, &IssueWidget::TaskDeleted, this, &MainWindow::OnTransition);

    connect(&Backend::Instance, &Backend::LoadingChanged, this, &MainWindow::OnLoadingChanged);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::OnTransition(MainWindow::Transition transition, const Context& context) {
    myContext = context;

    switch (transition) {
    case Transition::Greetings:
        Backend::Instance.GetProjects();
        break;
    }

    AbstractPage* page = (AbstractPage*)ui->stackedWidget->widget((int)transition);
    page->Setup(myContext);

    myTransitionsHistory.push(QPair<Transition, Context>((Transition)ui->stackedWidget->currentIndex(), myContext));
    ui->stackedWidget->setCurrentIndex((int)transition);
}

void MainWindow::OnBackButtonClicked() {
    if (myTransitionsHistory.empty()) {
        return;
    }

    QPair<Transition, Context> prevState = myTransitionsHistory.pop();
    OnTransition(prevState.first, prevState.second);
    myTransitionsHistory.pop();
}

void MainWindow::OnLoadingChanged(bool isLoading) {
    if (isLoading) {
        ui->loadingBar->StartLoading();
    } else {
        ui->loadingBar->StopLoading();
    }
}

void MainWindow::OnLogout() {
    myAuthWindow.show();
    this->close();
}

void MainWindow::OnNavBarTransition(MainWindow::Transition transition) {
    OnTransition(transition, myContext);
}

