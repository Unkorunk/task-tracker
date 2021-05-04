#include "ProjectsList.h"
#include "ui_ProjectsList.h"

#include <QDialog>

ProjectsList::ProjectsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectsList),
    model(std::make_unique<QStringListModel>())
{
    ui->setupUi(this);

    //model->setStringList(QStringList { "a", "b", "c"});

    ui->listView->setModel(model.get());

    connect(ui->addProjectBtn, &QAbstractButton::clicked, this, &ProjectsList::OnAddProjectBtnClicked);
    connect(ui->listView, &QListView::clicked, this, &ProjectsList::OnItemClicked);
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
    QStringList stringList;
    for (auto& project : myProjects) {
        stringList.append(project.projectName);
    }

    model->setStringList(stringList);
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
    emit AddProjectClicked(QString("NewProject%1").arg(model->rowCount()));
}

void ProjectsList::OnItemClicked(const QModelIndex &index)
{
    emit ProjectSelected(myProjects[index.row()]);
}
