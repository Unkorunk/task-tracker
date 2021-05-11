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

private slots:
    void OnEditClicked();
    void OnSaveClicked();

    void OnTaskUpdated(Status status);

public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

    void SetupTask(const TaskInfo& task);

private:
    void ToEditMode();
    void ToReadOnlyMode();

    void LockUi();
    void UnlockUi();

    Ui::IssueWidget *ui;
    bool edited;
    TaskInfo task_info;
    //QString text;
};

#endif // ISSUEWIDGET_H
