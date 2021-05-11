#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>
#include "Backend.h"

namespace Ui {
class IssueWidget;
}

class IssueWidget : public QWidget
{
    Q_OBJECT

public slots:
    void OnEdit();
    void OnSave();
    //void OnOpen();

public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

    void SetupTask(const TaskInfo& task);

private:
    Ui::IssueWidget *ui;
    bool edited;
    TaskInfo task_info;
    //QString text;
};

#endif // ISSUEWIDGET_H
