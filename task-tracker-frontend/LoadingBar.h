#ifndef LOADINGBAR_H
#define LOADINGBAR_H

#include <QWidget>

namespace Ui {
class LoadingBar;
}

class LoadingBar : public QWidget
{
    Q_OBJECT

public slots:
    void FailLoading(QString log);

public:
    explicit LoadingBar(QWidget *parent = nullptr);
    ~LoadingBar();

    void StartLoading();

    void StopLoading();

private:
    Ui::LoadingBar *ui;

    bool isError = false;
};

#endif // LOADINGBAR_H
