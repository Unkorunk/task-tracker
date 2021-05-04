#include "ProjectsList.h"
#include "ui_ProjectsList.h"

ProjectsList::ProjectsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectsList)
{
    ui->setupUi(this);
}

ProjectsList::~ProjectsList()
{
    delete ui;
}

void ProjectsList::ChangeHeader(const QString& header)
{
    ui->projectListHeader->setText(header);
}

void ProjectsList::OnAddProjectBtnClicked()
{
// TODO: make it work
}
