#ifndef LABELPROPERTYWIDGET_H
#define LABELPROPERTYWIDGET_H

#include <QWidget>

namespace Ui {
class LabelPropertyWidget;
}

class LabelPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LabelPropertyWidget(QWidget *parent = nullptr);
    ~LabelPropertyWidget();

    void SetData(const QString& caption, const QString& data);

private:
    Ui::LabelPropertyWidget *ui;
};

#endif // LABELPROPERTYWIDGET_H
