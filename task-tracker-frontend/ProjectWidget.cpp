#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"

#include <QDebug>

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget),
    model(std::make_unique<QStringListModel>())
{
    ui->setupUi(this);

    ui->listView->setModel(model.get());

    connect(ui->CreateTaskBtn, &QAbstractButton::clicked, this, &ProjectWidget::OnCreateTaskBtnClicked);
    connect(ui->listView, &QListView::clicked, this, &ProjectWidget::OnItemClicked);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::OnCreateTaskBtnClicked()
{
    model->insertRow(model->rowCount());
    QModelIndex idx = model->index(model->rowCount() - 1);
    model->setData(idx, "testsss");

//    if (myDialog.get() != nullptr && myDialog->isVisible()) {
//        myDialog->close();
//    }

//    myDialog = std::make_unique<QDialog>(this->parent());
//    QVBoxLayout dialogLayout;
//    dialogLayout.addItem(new QTextBlock());

//    myDialog->setLayout()
 //   emit CreateTaskClicked(QString("NewTask%1").arg(model->rowCount()));
}

void ProjectWidget::OnItemClicked(const QModelIndex &index)
{
    emit TaskSelected(myTasks[index.row()]);
}


void ProjectWidget::SetupProject(const ProjectInfo &project)
{
    qInfo() << project.projectName;
}
