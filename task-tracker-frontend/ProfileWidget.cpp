#include <QNetworkReply>
#include "ProfileWidget.h"
#include "ui_ProfileWidget.h"

ProfileWidget::ProfileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileWidget),
    user_info(-1, "", "", "")
{
    ui->setupUi(this);
    this->SetupProfile(user_info);
}

ProfileWidget::~ProfileWidget()
{
    delete ui;
}

void ProfileWidget::SetupProfile(const UserInfo& user)
{
    user_info = user;

    // Is it correct to set ui parameters here?
    this->ui->loginField->setText(user_info.GetUsername());
    this->ui->fullNameField->setText(user_info.GetFullName());
    this->ui->eMailField->setText(user_info.GetEmail());

    std::string avatarUrl = "http://tinygraphs.com/squares/" + user_info.GetUsername().toStdString()
            +"?theme=frogideas&numcolors=4&size=50&fmt=svg";
    LoadAvatar(avatarUrl);
}

void ProfileWidget::LoadAvatar(const std::string &strAvatarUrl)
{
    QUrl url(QString().fromStdString(strAvatarUrl));
    QNetworkAccessManager manager;
    QEventLoop loop;

    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, &QNetworkReply::finished, &loop, [&reply, &loop, this](){
     if (reply->error() == QNetworkReply::NoError)
     {
         QByteArray jpegData = reply->readAll();
         QPixmap pixmap;
         pixmap.loadFromData(jpegData);
         if (!pixmap.isNull())
         {
             this->ui->imgLbl->clear();
             this->ui->imgLbl->setPixmap(pixmap);
         }
     }
     loop.quit();
   });

   loop.exec();
}
