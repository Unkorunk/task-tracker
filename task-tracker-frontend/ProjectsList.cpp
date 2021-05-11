#include "ProjectsList.h"
#include "ui_ProjectsList.h"
#include "ProjectItemWidget.h"
#include <QDialog>

ProjectsList::ProjectsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectsList)
{
    ui->setupUi(this);

    connect(ui->addProjectBtn, &QAbstractButton::clicked, this, &ProjectsList::OnAddProjectBtnClicked);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));
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
       widget->setProject(project.projectName);
       item->setSizeHint(QSize(200, 50));

       ui->listWidget->addItem(item);
       ui->listWidget->setItemWidget(item, widget);
       update();
    }
}

void ProjectsList::OnAddProjectBtnClicked()
{
//    model->insertRow(model->rowCount());
//    QModelIndex idx = model->index(model->rowCount() - 1);
//    model->setData(idx, "testsss");

//    if (myDialog.get() != nullptr && myDialog->isVisible()) {
//        myDialog->close();
//    }

//    myDialog = std::make_unique<QDialog>(this->parent());
//    QVBoxLayout dialogLayout;
//    dialogLayout.addItem(new QTextBlock());

//    myDialog->setLayout()
    emit AddProjectClicked(QString("NewProject%1").arg(ui->listWidget->count()));
}

void ProjectsList::OnItemClicked(QListWidgetItem* item)
{
    auto index = ui->listWidget->indexFromItem(item);
    qInfo() << index.row();
    emit ProjectSelected(myProjects[index.row()]);
}
