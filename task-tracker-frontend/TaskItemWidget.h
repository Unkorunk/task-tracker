#ifndef TASKITEMWIDGET_H
#define TASKITEMWIDGET_H

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
    void setTask(const QString &text);
    QString getTask();

private:
    Ui::TaskItemWidget *ui;
};

#endif // TASKITEMWIDGET_H