#include "TaskItemWidget.h"
#include "ui_TaskItemWidget.h"
#include <QGraphicsDropShadowEffect>

TaskItemWidget::TaskItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ui->endDate->setSpecialValueText("No deadline");

    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setColor(QColor("#C0C0C0"));
    ui->widget->setGraphicsEffect(effect);
}

TaskItemWidget::~TaskItemWidget()
{
    delete ui;
}

void TaskItemWidget::SetTask(const TaskInfo &task)
{
    ui->taskName->setText(task.GetTitle());
    ui->startDate->setDateTime(task.GetCreationTime());

    std::optional<QDateTime> deadline = task.GetDeadline();
    if (deadline.has_value()) {
        ui->endDate->setDateTime(deadline.value());
    } else {
        ui->endDate->setDate(ui->endDate->minimumDate());
    }
}
