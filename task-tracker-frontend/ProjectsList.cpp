#include "ProjectsList.h"
#include "ui_ProjectsList.h"
#include "ProjectItemWidget.h"
#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include <QScrollBar>

ProjectsList::ProjectsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectsList) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ui->listWidget->verticalScrollBar()->setSingleStep(2);
    dialog = new CreateProjectDialog(this);

    dialog->setModal(true);

    dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    connect(ui->addProjectBtn, &QAbstractButton::clicked, this, &ProjectsList::OnAddProjectBtnClicked);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT (IfTextChanged()));

    connect(dialog, SIGNAL(createProject(QString&)), this, SLOT(OnProjectCreated(QString&)));

}

ProjectsList::~ProjectsList() {
    delete ui;
}


void ProjectsList::ChangeHeader(const QString& header) {
    ui->projectListHeader->setText(header);
}

void ProjectsList::SetProjects(const QList<QPair<ProjectInfo, RoleInfo>>& list) {
    myProjects = list;
    ui->listWidget->clear();
    for (auto& project : myProjects) {
       auto item = new QListWidgetItem();
       auto widget = new ProjectItemWidget(this);
       widget->setProject(project.first.GetTitle());
       item->setSizeHint(QSize(200, 70));
       ui->listWidget->addItem(item);
       ui->listWidget->setItemWidget(item, widget);
       update();
    }
}

void ProjectsList::OnProjectCreated(QString& projectName) {
    emit AddProjectClicked(projectName);
}

void ProjectsList::OnAddProjectBtnClicked() {
    dialog->clear();
    dialog->show();
}

void ProjectsList::IfTextChanged(){

    QStringList query_parts = ui->textEdit->toPlainText().toLower().trimmed().split(" ", Qt::SkipEmptyParts, Qt::CaseInsensitive);

    for(int i = 0; i < this->myProjects.size(); i++){

        QStringList title_parts =
                this->myProjects[i].first.GetTitle().toLower().trimmed().split(" ", Qt::SkipEmptyParts, Qt::CaseInsensitive);

        bool is_title_contains_query = true;

        for (int k = 0; k < query_parts.size(); k++){

            bool is_query_part_find = false;

            for (int j = 0; j < title_parts.size(); j++){

                if (title_parts[j].indexOf(query_parts[k]) == 0) {
                    is_query_part_find = true;
                }

            }

            if (!is_query_part_find) {
                is_title_contains_query = false;
                break;
            }

        }
        ui->listWidget->item(i)->setHidden(!is_title_contains_query);
        update();
    }
}

void ProjectsList::OnItemClicked(QListWidgetItem* item) {
    auto index = ui->listWidget->indexFromItem(item);
    emit ProjectSelected(myProjects[index.row()].first, myProjects[index.row()].second);
}
