#ifndef VALUESELECTORWIDGET_H
#define VALUESELECTORWIDGET_H

#include "DataClasses.h"

#include <QWidget>

namespace Ui {
class ValueSelectorWidget;
}

class ValueSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ValueSelectorWidget(QWidget *parent = nullptr);
    ~ValueSelectorWidget();

    void SetupTags(const QList<TagInfo>& tags, const TaskInfo& task);
    void SetupTag(const TaskTag& tag);

private slots:
    void OnAddDeleteClicked();
    void OnPropertyChanged(int idx);

private:
    QList<TagInfo> myTags;
    TaskInfo myTask;
    TaskTag myTaskTag;
    bool isAdded;

    Ui::ValueSelectorWidget *ui;
};

#endif // VALUESELECTORWIDGET_H
