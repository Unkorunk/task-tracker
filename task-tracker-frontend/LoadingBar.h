#ifndef LOADINGBAR_H
#define LOADINGBAR_H

#include <QWidget>

namespace Ui {
class LoadingBar;
}

class LoadingBar : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingBar(QWidget *parent = nullptr);
    ~LoadingBar();

    void StartLoading();

    void StopLoading();

private:
    Ui::LoadingBar *ui;
};

#endif // LOADINGBAR_H
