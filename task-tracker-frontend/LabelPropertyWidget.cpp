#include "LabelPropertyWidget.h"
#include "ui_LabelPropertyWidget.h"

LabelPropertyWidget::LabelPropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabelPropertyWidget) {
    ui->setupUi(this);
}

LabelPropertyWidget::~LabelPropertyWidget() {
    delete ui;
}

void LabelPropertyWidget::SetData(const QString &caption, const QString &data) {
    ui->captionLabel->setText(caption);
    ui->dataLabel->setText(data);
}
