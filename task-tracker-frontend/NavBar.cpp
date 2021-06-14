#include "NavBar.h"
#include "ui_NavBar.h"

#include <QTimer>

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

    connect(&Backend::Instance, &Backend::UnreadLoaded, this, &NavBar::OnUnreadsLoaded);
    connect(&Backend::Instance, &Backend::NotificationsLoaded, this, &NavBar::OnNotificationsLoaded);
    connect(&Backend::Instance, &Backend::SignedIn, this, &NavBar::OnSignedIn);
    connect(&Backend::Instance, &Backend::SignedUp, this, &NavBar::OnSignedIn);

    myTimer = new QTimer(this);
    QObject::connect(myTimer, &QTimer::timeout, this, &NavBar::OnInfoUpdateTime);
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

void NavBar::OnSignedIn(Status status)
{
    if (status.isSuccess) {
        OnInfoUpdateTime();
        myTimer->start(30000);
    }
}

void NavBar::OnInfoUpdateTime()
{
    Backend::Instance.GetNotifications();
}

void NavBar::OnNotificationsLoaded(Status status, const QList<NotificationInfo> &notifications)
{
    int unreads = 0;
    for (auto& it : notifications) {
        if (!it.IsRead()) {
            unreads++;
        }
    }

    if (unreads == 0) {
        ui->notificationsCounter->hide();
        ui->notificationsCounter->setText("");
    } else {
        ui->notificationsCounter->setText(QString("%1").arg(unreads));
        ui->notificationsCounter->show();
    }
}

void NavBar::OnUnreadsLoaded(Status status, const QList<NotificationInfo> &notifications)
{
    if (status.isSuccess) {
        ui->notificationsCounter->hide();
        ui->notificationsCounter->setText("");
    }
}
