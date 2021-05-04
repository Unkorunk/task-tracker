#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->navBar, SIGNAL(OnNavbarClicked(MainWindow::Transition)), this, SLOT(OnTransition(MainWindow::Transition)));
    //connect( ui->pushButton, SIGNAL(clicked()), this, SLOT(OnSayHello()) );
}

void MainWindow::OnSayHello() {
    //ui->label->setText("Hello World");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnTransition(MainWindow::Transition transition)
{
    ui->stackedWidget->setCurrentIndex((int)transition);
}

