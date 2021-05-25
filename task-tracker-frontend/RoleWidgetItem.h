#ifndef ROLEWIDGETITEM_H
#define ROLEWIDGETITEM_H

#include "DataClasses.h"

#include <QWidget>

namespace Ui {
class RoleWidgetItem;
}

class RoleWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit RoleWidgetItem(QWidget *parent = nullptr);
    ~RoleWidgetItem();

    RoleInfo GetRole() const;
    void SetRole(const RoleInfo& role);

    void Clear();

    void EditMode();
    void ReadonlyMode();

private slots:
    void OnEditCancelBtnClicked();
    void OnSaveBtnClicked();
    void OnDeleteBtnClicked();

private:
    mutable RoleInfo myRole;
    bool isEditable;

    Ui::RoleWidgetItem *ui;
};

#endif // ROLEWIDGETITEM_H
