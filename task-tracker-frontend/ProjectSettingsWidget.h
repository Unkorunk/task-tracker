#ifndef PROJECTSETTINGSWIDGET_H
#define PROJECTSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectSettingsWidget;
}

class ProjectSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSettingsWidget(QWidget *parent = nullptr);
    ~ProjectSettingsWidget();

private:
    Ui::ProjectSettingsWidget *ui;
};

#endif // PROJECTSETTINGSWIDGET_H
