#ifndef NAVBAR_H
#define NAVBAR_H

#include <QGroupBox>

namespace Ui {
class NavBar;
}

class NavBar : public QGroupBox
{
    Q_OBJECT

public:
    explicit NavBar(QWidget *parent = nullptr);
    ~NavBar();

private:
    Ui::NavBar *ui;
};

#endif // NAVBAR_H
