#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //connect( ui->pushButton, SIGNAL(clicked()), this, SLOT(OnSayHello()) );
}

void MainWindow::OnSayHello() {
    //ui->label->setText("Hello World");
}

MainWindow::~MainWindow()
{
    delete ui;
}

