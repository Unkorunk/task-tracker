#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include "backend.h"

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    ~StatisticsWidget();

    void SetupProject(const ProjectInfo& project);
private:
    Ui::StatisticsWidget *ui;
};

#endif // STATISTICSWIDGET_H
