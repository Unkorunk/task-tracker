#include "TaskItemWidget.h"
#include "ui_TaskItemWidget.h"

TaskItemWidget::TaskItemWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::TaskItemWidget) {
    ui->setupUi(this);
    ui->endDate->setSpecialValueText("No deadline");
}

TaskItemWidget::~TaskItemWidget() {
    delete ui;
}

void TaskItemWidget::SetTask(const TaskInfo &task) {
    ui->taskName->setText(task.GetTitle());
    ui->startDate->setDateTime(task.GetCreationTime());

    std::optional<QDateTime> deadline = task.GetDeadline();
    if (deadline.has_value()) {
        ui->endDate->setDateTime(deadline.value());
    } else {
        ui->endDate->setDate(ui->endDate->minimumDate());
    }
}
