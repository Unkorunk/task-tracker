#include "DateTimePropertyWidget.h"
#include "ui_DateTimePropertyWidget.h"

QDate DateTimePropertyWidget::EmptyDate = QDate::fromString( "01/01/0001", "dd/MM/yyyy" );

DateTimePropertyWidget::DateTimePropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateTimePropertyWidget) {
    ui->setupUi(this);
    ui->dateTimeEdit->setSpecialValueText("No date");
    ui->dateTimeEdit->setDate(EmptyDate);
    SetEditable(false);
}

DateTimePropertyWidget::~DateTimePropertyWidget() {
    delete ui;
}

void DateTimePropertyWidget::SetData(const QString &caption, const std::optional<QDateTime> &data) {
    ui->captionLabel->setText(caption);
    if (data.has_value()) {
        ui->dateTimeEdit->setDateTime(data.value());
        ui->dateTimeEdit->show();
    } else {
        ui->dateTimeEdit->clear();
        ui->dateTimeEdit->hide();
    }
}

std::optional<QDateTime> DateTimePropertyWidget::GetData() const {
    std::optional<QDateTime> res;
    if (ui->dateTimeEdit->date() != EmptyDate) {
        res.emplace(ui->dateTimeEdit->dateTime());
    }
    return res;
}

void DateTimePropertyWidget::SetEditable(bool editable) {
    isEditable = editable;
    ui->dateTimeEdit->setEnabled(isEditable);
    if (isEditable) {
        ui->dateTimeEdit->show();
    } else if (ui->dateTimeEdit->date() == EmptyDate) {
        ui->dateTimeEdit->hide();
    }
}

bool DateTimePropertyWidget::GetEditable() const {
    return isEditable;
}
