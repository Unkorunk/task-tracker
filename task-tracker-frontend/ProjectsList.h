#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QWidget>

namespace Ui {
class ProjectsList;
}

class ProjectsList : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectsList(QWidget *parent = nullptr);
    ~ProjectsList();

    void ChangeHeader(const QString& header);

private slots:
    void OnAddProjectBtnClicked();

private:
    Ui::ProjectsList *ui;
};

#endif // PROJECTSLIST_H
