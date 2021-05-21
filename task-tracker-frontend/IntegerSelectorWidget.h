#ifndef INTEGERSELECTORWIDGET_H
#define INTEGERSELECTORWIDGET_H

#include <QWidget>

namespace Ui {
class IntegerSelectorWidget;
}

class IntegerSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntegerSelectorWidget(QWidget *parent = nullptr);
    ~IntegerSelectorWidget();

    void SetData(const QString& caption, int data);
    void ChangeData(int data);
    int GetData() const;

    void SetEditable(bool editable);
    bool GetEditable() const;

private:
    Ui::IntegerSelectorWidget *ui;
    bool isEditable;
};

#endif // INTEGERSELECTORWIDGET_H
