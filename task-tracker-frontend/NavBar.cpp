#include "NavBar.h"
#include "ui_NavBar.h"

NavBar::NavBar(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::NavBar)
{
    ui->setupUi(this);
}

NavBar::~NavBar()
{
    delete ui;
}
