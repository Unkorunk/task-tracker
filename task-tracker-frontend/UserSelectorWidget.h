#ifndef USERSELECTORWIDGET_H
#define USERSELECTORWIDGET_H

#include "DataClasses.h"

#include <QWidget>

namespace Ui {
class UserSelectorWidget;
}

class UserSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserSelectorWidget(QWidget *parent = nullptr);
    ~UserSelectorWidget();

    void SetData(const QString& caption, const std::optional<UserInfo>& selectedUser, const QList<UserInfo>& users);
    void ChangeData(const std::optional<UserInfo>& selectedUser);
    std::optional<UserInfo> GetData() const;

    void SetEditable(bool editable);
    bool GetEditable() const;

    bool IsEmpty() const;

private:
    Ui::UserSelectorWidget *ui;
    bool isEditable;

    QList<UserInfo> myUsers;
};

#endif // USERSELECTORWIDGET_H
