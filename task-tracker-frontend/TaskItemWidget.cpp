#include "TaskItemWidget.h"
#include "ui_TaskItemWidget.h"

TaskItemWidget::TaskItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
}

TaskItemWidget::~TaskItemWidget()
{
    delete ui;
}

void TaskItemWidget::setTask(const QString &text)
{
    ui->taskName->setText(text);
}
QString TaskItemWidget::getTask()
{
    return ui->taskName->text();
}
