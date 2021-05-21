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
    std::optional<QDateTime> GetData() const;

    void SetEditable(bool editable);
    bool GetEditable() const;

private:
    static QDate EmptyDate;

    Ui::DateTimePropertyWidget *ui;
    bool isEditable;
};

#endif // DATETIMEPROPERTYWIDGET_H
