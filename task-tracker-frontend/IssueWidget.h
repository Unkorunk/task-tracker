#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>
#include "backend.h"

namespace Ui {
class IssueWidget;
}

class IssueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

    void SetupTask(const TaskInfo& task);

private:
    Ui::IssueWidget *ui;
};

#endif // ISSUEWIDGET_H
