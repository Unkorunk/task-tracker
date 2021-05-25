#ifndef TASKITEMWIDGET_H
#define TASKITEMWIDGET_H

#include "DataClasses.h"

#include <QWidget>

namespace Ui {
class TaskItemWidget;
}

class TaskItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskItemWidget(QWidget *parent = nullptr);
    ~TaskItemWidget();
    void SetTask(const TaskInfo &task);

private:
    Ui::TaskItemWidget *ui;
};

#endif // TASKITEMWIDGET_H
