#ifndef PROJECTITEMWIDGET_H
#define PROJECTITEMWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectItemWidget;
}

class ProjectItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectItemWidget(QWidget *parent = nullptr);
    ~ProjectItemWidget();
    void setProject(const QString &text);
    QString getProject();

private:
    Ui::ProjectItemWidget *ui;
};

#endif // PROJECTITEMWIDGET_H
