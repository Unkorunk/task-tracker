#include "TaskItemWidget.h"
#include "ui_TaskItemWidget.h"
#include <QGraphicsDropShadowEffect>

TaskItemWidget::TaskItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ui->endDate->setText("No deadline");
    ui->endDate->setText("No deadline");
}

TaskItemWidget::~TaskItemWidget()
{
    delete ui;
}

void TaskItemWidget::SetTask(const TaskInfo &task)
{
    ui->taskName->setText(task.GetTitle());
    ui->startDate->setText((task.GetCreationTime()).toString("dd.MM.yyyy"));

    std::optional<QDateTime> deadline = task.GetDeadline();
    if (deadline.has_value()) {
        ui->endDate->setText((deadline.value()).toString("dd.MM.yyyy"));
    } else {
        ui->endDate->setText("No deadline");
    }
}
