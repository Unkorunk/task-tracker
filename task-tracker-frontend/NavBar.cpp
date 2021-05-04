#include "NavBar.h"
#include "ui_NavBar.h"

NavBar::NavBar(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::NavBar)
{
    ui->setupUi(this);
    connect(ui->projectsBtn, SIGNAL(clicked()), this, SLOT(OnProjectBtnClicked()));
    connect(ui->notificationBtn, SIGNAL(clicked()), this, SLOT(OnNotificationBtnClicked()));
    connect(ui->profileBtn, SIGNAL(clicked()), this, SLOT(OnProfileBtnClicked()));
    connect(ui->logOutBtn, SIGNAL(clicked()), this, SLOT(OnLogoutBtnClicked()));
}

NavBar::~NavBar()
{
    delete ui;
}

void NavBar::OnProjectBtnClicked()
{
    emit OnNavbarClicked(MainWindow::Transition::Greetings);
}

void NavBar::OnNotificationBtnClicked()
{
    emit OnNavbarClicked(MainWindow::Transition::Notifications);
}

void NavBar::OnProfileBtnClicked()
{
    emit OnNavbarClicked(MainWindow::Transition::Profile);
}

void NavBar::OnLogoutBtnClicked()
{
    emit OnLogout();
}
