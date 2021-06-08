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
    connect(ui->backButton, &QAbstractButton::clicked, this, &NavBar::OnBackBtnClicked);
}

NavBar::~NavBar()
{
    delete ui;
}

void NavBar::OnProjectBtnClicked()
{
    emit NavButtonClicked(MainWindow::Transition::Greetings);
}

void NavBar::OnNotificationBtnClicked()
{
    emit NavButtonClicked(MainWindow::Transition::Notifications);
}

void NavBar::OnProfileBtnClicked()
{
    emit NavButtonClicked(MainWindow::Transition::Profile);
}

void NavBar::OnLogoutBtnClicked()
{
    emit Logout();
}

void NavBar::OnBackBtnClicked()
{
    emit BackButtonClicked();
}
