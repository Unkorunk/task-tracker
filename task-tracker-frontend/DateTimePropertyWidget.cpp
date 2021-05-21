#include "DateTimePropertyWidget.h"
#include "ui_DateTimePropertyWidget.h"


DateTimePropertyWidget::DateTimePropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateTimePropertyWidget) {
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
    ui->dateTimeEdit->hide();
    isEmpty = true;
    SetEditable(false);
}

DateTimePropertyWidget::~DateTimePropertyWidget() {
    delete ui;
}

void DateTimePropertyWidget::SetData(const QString &caption, const std::optional<QDateTime> &data) {
    ui->captionLabel->setText(caption);
    ChangeData(data);
}

void DateTimePropertyWidget::ChangeData(const std::optional<QDateTime> &data) {
    if (data.has_value()) {
        ui->dateTimeEdit->setDateTime(data.value());
        ui->dateTimeEdit->show();
    } else {
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
        ui->dateTimeEdit->hide();
    }
    isEmpty = !data.has_value();
}

std::optional<QDateTime> DateTimePropertyWidget::GetData() const {
    std::optional<QDateTime> res;
    if (!isEmpty) {
        res.emplace(ui->dateTimeEdit->dateTime());
    }
    return res;
}

void DateTimePropertyWidget::SetEditable(bool editable) {
    isEditable = editable;
    ui->dateTimeEdit->setEnabled(isEditable);
    if (editable) {
        isEmpty = false;
        ui->dateTimeEdit->show();
    } else if (isEmpty) {
        ui->dateTimeEdit->hide();
    }
}

bool DateTimePropertyWidget::GetEditable() const {
    return isEditable;
}
