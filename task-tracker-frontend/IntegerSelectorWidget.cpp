#include "IntegerSelectorWidget.h"
#include "ui_IntegerSelectorWidget.h"

IntegerSelectorWidget::IntegerSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntegerSelectorWidget) {
    ui->setupUi(this);
    SetEditable(false);
}

IntegerSelectorWidget::~IntegerSelectorWidget() {
    delete ui;
}

void IntegerSelectorWidget::SetData(const QString &caption, int data) {
    ui->captionLabel->setText(caption);
    ChangeData(data);
}

void IntegerSelectorWidget::ChangeData(int data) {
    ui->integerSelector->setValue(data);
}

int IntegerSelectorWidget::GetData() const {
    return ui->integerSelector->value();
}

void IntegerSelectorWidget::SetEditable(bool editable) {
    isEditable = editable;
    ui->integerSelector->setEnabled(editable);
}

bool IntegerSelectorWidget::GetEditable() const {
    return isEditable;
}


