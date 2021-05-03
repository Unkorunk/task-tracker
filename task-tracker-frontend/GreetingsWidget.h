#ifndef GREETINGSWIDGET_H
#define GREETINGSWIDGET_H

#include <QWidget>

namespace Ui {
class GreetingsWidget;
}

class GreetingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GreetingsWidget(QWidget *parent = nullptr);
    ~GreetingsWidget();

private:
    Ui::GreetingsWidget *ui;
};

#endif // GREETINGSWIDGET_H
