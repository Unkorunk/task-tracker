#include "Backend.h"
#include "PropertyValueItemWidget.h"
#include "ui_PropertyValueItemWidget.h"

PropertyValueItemWidget::PropertyValueItemWidget(QWidget *parent) :
        QWidget(parent), myValue(-1, ""), ui(new Ui::PropertyValueItemWidget) {
    ui->setupUi(this);

    connect(ui->deleteBtn, &QAbstractButton::clicked, this, &PropertyValueItemWidget::OnDeleteClicked);
}

PropertyValueItemWidget::~PropertyValueItemWidget() {
    delete ui;
}

void PropertyValueItemWidget::SetupValue(const TagValue &tagValue) {
    myValue = tagValue;
    ui->label->setText(tagValue.GetValue());
}

void PropertyValueItemWidget::OnDeleteClicked() {
    Backend::Instance.DeleteTagValue(myValue);
}
