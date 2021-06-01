#ifndef PROPERTYVALUEITEMWIDGET_H
#define PROPERTYVALUEITEMWIDGET_H

#include <QWidget>
#include "DataClasses.h"

namespace Ui {
class PropertyValueItemWidget;
}

class PropertyValueItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyValueItemWidget(QWidget *parent = nullptr);
    ~PropertyValueItemWidget();

    void SetupValue(const TagValue& tagValue, bool isDeletable);

private slots:
    void OnDeleteClicked();

private:
    TagValue myValue;

    Ui::PropertyValueItemWidget *ui;
};

#endif // PROPERTYVALUEITEMWIDGET_H
