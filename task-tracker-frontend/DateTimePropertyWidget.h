#ifndef DATETIMEPROPERTYWIDGET_H
#define DATETIMEPROPERTYWIDGET_H

#include <QWidget>

namespace Ui {
class DateTimePropertyWidget;
}

class DateTimePropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateTimePropertyWidget(QWidget *parent = nullptr);
    ~DateTimePropertyWidget();

    void SetData(const QString& caption, const std::optional<QDateTime> &data);
    void ChangeData(const std::optional<QDateTime> &data);
    std::optional<QDateTime> GetData() const;

    void SetEditable(bool editable);
    bool GetEditable() const;

private:
    Ui::DateTimePropertyWidget *ui;
    bool isEditable;
    bool isEmpty;
};

#endif // DATETIMEPROPERTYWIDGET_H
