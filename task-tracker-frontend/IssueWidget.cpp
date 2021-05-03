#include "IssueWidget.h"
#include "ui_IssueWidget.h"

IssueWidget::IssueWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IssueWidget)
{
    ui->setupUi(this);
}

IssueWidget::~IssueWidget()
{
    delete ui;
}
