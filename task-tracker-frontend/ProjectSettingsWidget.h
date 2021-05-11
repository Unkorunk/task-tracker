#ifndef PROJECTSETTINGSWIDGET_H
#define PROJECTSETTINGSWIDGET_H

#include <QWidget>
#include "Backend.h"

namespace Ui {
class ProjectSettingsWidget;
}

class ProjectSettingsWidget : public QWidget
{
    Q_OBJECT

private slots:
    void OnSaveProjectName(const ProjectInfo &project);

public:
    explicit ProjectSettingsWidget(QWidget *parent = nullptr);
    ~ProjectSettingsWidget();

    void SetupProject(const ProjectInfo& project);

private:
    Ui::ProjectSettingsWidget *ui;
};

#endif // PROJECTSETTINGSWIDGET_H
