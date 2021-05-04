#include "GreetingsWidget.h"
#include "ui_GreetingsWidget.h"
#include "backend.h"

GreetingsWidget::GreetingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GreetingsWidget)
{
    ui->setupUi(this);
    ui->personalProjectsList->ChangeHeader("Personal projects");

    Backend::Instance.GetProjects();
}

GreetingsWidget::~GreetingsWidget()
{
    delete ui;
}
