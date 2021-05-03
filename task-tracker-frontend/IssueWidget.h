#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>

namespace Ui {
class IssueWidget;
}

class IssueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IssueWidget(QWidget *parent = nullptr);
    ~IssueWidget();

private:
    Ui::IssueWidget *ui;
};

#endif // ISSUEWIDGET_H
