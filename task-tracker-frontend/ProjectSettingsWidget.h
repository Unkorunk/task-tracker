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
    void OnSaveClicked();

    void OnCancelClicked();

    void OnProjectEdited(Status status);

public:
    explicit ProjectSettingsWidget(QWidget *parent = nullptr);
    ~ProjectSettingsWidget();

    void SetupProject(const ProjectInfo& project);

private:
    void ToEditMode();

    void ToReadonlyMode();

    void LockUi();

    void UnlockUi();

    Ui::ProjectSettingsWidget *ui;

    ProjectInfo myProject;
};

#endif // PROJECTSETTINGSWIDGET_H
