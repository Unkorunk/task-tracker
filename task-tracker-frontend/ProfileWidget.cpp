#include "ProfileWidget.h"
#include "ui_ProfileWidget.h"

ProfileWidget::ProfileWidget(QWidget *parent) :
    AbstractPage(parent),
    ui(new Ui::ProfileWidget) {
    ui->setupUi(this);
}

ProfileWidget::~ProfileWidget() {
    delete ui;
}

void ProfileWidget::SetupPage() { }
