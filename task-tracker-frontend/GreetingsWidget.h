#ifndef GREETINGSWIDGET_H
#define GREETINGSWIDGET_H

#include <QWidget>
#include "AbstractPage.h"
#include "Backend.h"
#include "MainWindow.h"

namespace Ui {
class GreetingsWidget;
}

class GreetingsWidget : public AbstractPage
{
    Q_OBJECT

public:
    explicit GreetingsWidget(QWidget *parent = nullptr);
    ~GreetingsWidget();

signals:
    void ProjectSelected(MainWindow::Transition transition, const Context& context);

protected:
    void SetupPage() override;

private slots:
    void OnProjectSelected(const ProjectInfo& project, const RoleInfo& role);

    void OnProjectsLoad(Status status, const QList<QPair<ProjectInfo, RoleInfo>>& projects);

    void OnProjectAdd(const QString& name);

    void OnProfileUpdate(Status status, const UserInfo& user);

private:
    Ui::GreetingsWidget *ui;
};

#endif // GREETINGSWIDGET_H
