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
    // TODO Make photo rendering
    //this->ui->avatarView->Set(user_info.GetPhoto());
    //this->ui->avatarView->setBackgroundBrush(QImage("https://www.philosophytalk.org/sites/default/files/styles/large_blog__900x400/public/graham-holtshausen-fUnfEz3VLv4-unsplash.jpg"));
}
