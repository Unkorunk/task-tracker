#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include "AbstractPage.h"
#include "Backend.h"

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public AbstractPage
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    ~StatisticsWidget();

protected:
    void SetupPage() override;

private:
    Ui::StatisticsWidget *ui;
};

#endif // STATISTICSWIDGET_H
