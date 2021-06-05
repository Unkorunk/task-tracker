#include <QNetworkReply>
#include <QMessageBox>
#include "ProfileWidget.h"
#include "ui_ProfileWidget.h"
#include "Backend.h"
#include "MainWindow.h"

ProfileWidget::ProfileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileWidget),
    user_info(-1, "", "", "")
{
    ui->setupUi(this);

    this->SetupProfile(user_info);

    connect(ui->EditUserInfoBtn, SIGNAL(clicked()), this, SLOT(OnEditUserInfoBtnClicked()));
    connect(ui->ResetPasswordBtn, SIGNAL(clicked()), this, SLOT(OnResetPasswordBtnClicked()));
    connect(&Backend::Instance, &Backend::Checked, this, &ProfileWidget::OnCheck);
    connect(ui->DeleteAccountBtn, SIGNAL(clicked()), this, SLOT(OnDeleteAccountBtnClicked()));
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

void ProfileWidget::OnEditUserInfoBtnClicked()
{
    if (this->ui->EditUserInfoBtn->text() == "Edit" &&
            !this->ui->fullNameField->isEnabled() && !this->ui->eMailField->isEnabled())
    {
        this->ui->EditUserInfoBtn->setText("Submit changes");
        this->ui->fullNameField->setEnabled(true);
        this->ui->eMailField->setEnabled(true);
    }
    else
    {
        this->ui->EditUserInfoBtn->setText("Edit");
        this->ui->fullNameField->setEnabled(false);
        this->ui->eMailField->setEnabled(false);

        user_info.SetEmail(this->ui->eMailField->text());
        user_info.SetFullName(this->ui->fullNameField->text());

        Backend::Instance.UpdateProfile(user_info);
    }
}

void ProfileWidget::OnResetPasswordBtnClicked()
{
    if (this->ui->newPasswordField->text() != this->ui->rptNewPasswordField->text())
    {
        return;
    }

    this->ui->currPasswordField->setEnabled(false);
    this->ui->newPasswordField->setEnabled(false);
    this->ui->rptNewPasswordField->setEnabled(false);

    Backend::Instance.CheckPassword(user_info.GetUsername(), this->ui->currPasswordField->text());
}

void ProfileWidget::OnCheck(Status status)
{
    if (status.isSuccess)
    {
        Backend::Instance.ResetPassword(this->ui->newPasswordField->text());
        this->ui->currPasswordField->clear();
        this->ui->newPasswordField->clear();
        this->ui->rptNewPasswordField->clear();
    }
    this->ui->currPasswordField->setEnabled(true);
    this->ui->newPasswordField->setEnabled(true);
    this->ui->rptNewPasswordField->setEnabled(true);
}

void ProfileWidget::OnDeleteAccountBtnClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Deleting account");
    msgBox.setInformativeText("Do you really want to delete this accoutn?");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        Backend::Instance.DeleteUser(user_info);
        emit Logout();
    }
}

//void ProfileWidget::OnUserDeleted(Status status)
//{
//    MainWindow::Instance->StopLoading();
//    if (status.isSuccess) {
//        MainWindow::Instance->OnTransition(MainWindow::Transition::Greetings);
//    }
//}
