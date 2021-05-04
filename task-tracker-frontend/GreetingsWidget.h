#ifndef GREETINGSWIDGET_H
#define GREETINGSWIDGET_H

#include <QWidget>
#include "backend.h"

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

private:
    Ui::GreetingsWidget *ui;

};

#endif // GREETINGSWIDGET_H
