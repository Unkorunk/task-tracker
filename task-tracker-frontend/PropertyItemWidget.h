#ifndef PROPERTYITEMWIDGET_H
#define PROPERTYITEMWIDGET_H

#include "DataClasses.h"

#include <QWidget>

namespace Ui {
class PropertyItemWidget;
}

class PropertyItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyItemWidget(QWidget *parent = nullptr);
    ~PropertyItemWidget();

    void SetupTag(const TagInfo& tag);
    TagInfo GetTag() const;

private slots:
    void OnDeleteBtnClicked();
    void OnSaveBtnClicked();
    void OnEditCancelBtnClicked();
    void OnAddValueClicked();

private:
    void SetEditable(bool editable);

    TagInfo myTag;
    bool isEditable;
    Ui::PropertyItemWidget *ui;
};

#endif // PROPERTYITEMWIDGET_H
