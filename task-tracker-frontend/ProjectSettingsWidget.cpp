#include "ProjectSettingsWidget.h"
#include "./ui_ProjectSettingsWidget.h"

ProjectSettingsWidget::ProjectSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSettingsWidget)
{
    ui->setupUi(this);
    setMinimumSize(600,600);
    setMaximumSize(1000,1000);
    //ui->label->setText(parent->objectName());

    //connect( ui->label, SIGNAL(clicked()), this, SLOT(OnNameProject()) );

}

//void ProjectSettingsWidget:: OnNameProject(){
//    ui->label->accessibleName();
//}

ProjectSettingsWidget::~ProjectSettingsWidget()
{
    delete ui;
}
