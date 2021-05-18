#include "CreateProjectDialog.h"
#include "ui_CreateProjectDialog.h"
#include <QGraphicsDropShadowEffect>
CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateProjectDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    ui->widget->setGraphicsEffect(effect);

    connect(ui->createBtn, &QPushButton::clicked, this, &CreateProjectDialog::onCreateProjectBtnClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &CreateProjectDialog::onCancelBtnClicked);
}

CreateProjectDialog::~CreateProjectDialog()
{
    delete ui;
}

void CreateProjectDialog::clear()
{
    ui->projectName->clear();
}

void CreateProjectDialog::onCreateProjectBtnClicked()
{
    qInfo() << ui->projectName->text();
    qInfo() << "create project";
    if (!ui->projectName->text().isEmpty()) {
        QString projectName(ui->projectName->text());
        emit createProject(projectName);
        this->accept();
    }

}

void CreateProjectDialog::onCancelBtnClicked()
{
    this->reject();
}
