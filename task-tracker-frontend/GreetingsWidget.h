#ifndef GREETINGSWIDGET_H
#define GREETINGSWIDGET_H

#include <QWidget>
#include "Backend.h"

namespace Ui {
class GreetingsWidget;
}

class GreetingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GreetingsWidget(QWidget *parent = nullptr);
    ~GreetingsWidget();

private slots:
    void OnProjectsLoad(Status status, const QList<ProjectInfo>& projects);

    void OnProjectAdd(const QString& name);

    void OnProfileUpdate(Status status, UserInfo user);

private:
    Ui::GreetingsWidget *ui;

};

#endif // GREETINGSWIDGET_H
