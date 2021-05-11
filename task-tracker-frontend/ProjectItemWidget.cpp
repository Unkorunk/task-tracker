#include "ProjectItemWidget.h"
#include "ui_ProjectItemWidget.h"

ProjectItemWidget::ProjectItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectItemWidget)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendRemoveItem(const QString &text)), parent, SLOT(removeItem(const QSTring &text)));
}

ProjectItemWidget::~ProjectItemWidget()
{
    delete ui;
}

void ProjectItemWidget::setProject(const QString &text)
{
    ui->projectName->setText(text);
}
QString ProjectItemWidget::getProject()
{
    return ui->projectName->text();
}

void ProjectItemWidget::onButtonClicked()
{
    emit SendRemoveItem(ui->projectName->text());
}
