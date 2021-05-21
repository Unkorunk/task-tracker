#include "ProjectsList.h"
#include "ui_ProjectsList.h"
#include "ProjectItemWidget.h"
#include <QDialog>
#include <QGraphicsDropShadowEffect>

ProjectsList::ProjectsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectsList)
{
    ui->setupUi(this);

    dialog = new CreateProjectDialog(this);

    dialog->setModal(true);
    dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    connect(ui->addProjectBtn, &QAbstractButton::clicked, this, &ProjectsList::OnAddProjectBtnClicked);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));

    connect(dialog, SIGNAL(createProject(QString&)), this, SLOT(OnProjectCreated(QString&)));
}

ProjectsList::~ProjectsList()
{
    delete ui;
}


void ProjectsList::ChangeHeader(const QString& header)
{
    ui->projectListHeader->setText(header);
}

void ProjectsList::SetProjects(const QList<ProjectInfo>& list)
{
    myProjects = list;
    ui->listWidget->clear();
    for (auto& project : myProjects) {
       auto item = new QListWidgetItem();
       auto widget = new ProjectItemWidget(this);
       widget->setProject(project.GetTitle());
       item->setSizeHint(QSize(200, 50));

       ui->listWidget->addItem(item);
       ui->listWidget->setItemWidget(item, widget);
       update();
    }
}

void  ProjectsList::OnProjectCreated(QString& projectName)
{
    emit AddProjectClicked(projectName);
}

void ProjectsList::OnAddProjectBtnClicked()
{
    dialog->clear();
    dialog->show();
}

void ProjectsList::OnItemClicked(QListWidgetItem* item)
{
    auto index = ui->listWidget->indexFromItem(item);
    emit ProjectSelected(myProjects[index.row()]);
}
