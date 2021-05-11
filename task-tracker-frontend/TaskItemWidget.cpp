#include "TaskItemWidget.h"
#include "ui_TaskItemWidget.h"

TaskItemWidget::TaskItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskItemWidget)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendRemoveItem(const QString &text)), parent, SLOT(removeItem(const QSTring &text)));

}

TaskItemWidget::~TaskItemWidget()
{
    delete ui;
}

void TaskItemWidget::setTask(const QString &text)
{
    ui->label->setText(text);
}
QString TaskItemWidget::getTask()
{
    return ui->label->text();
}
//void TaskItemWidget::SendRemoveItem(const QString &text)
//{

//}

void TaskItemWidget::onButtonClicked()
{
    emit SendRemoveItem(ui->label->text());
}
