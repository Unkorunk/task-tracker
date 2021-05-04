#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include "backend.h"

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    ~ProjectWidget();

    void SetupProject(const ProjectInfo& project);

private:
    Ui::ProjectWidget *ui;
};

#endif // PROJECTWIDGET_H
