#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include "AbstractPage.h"

#include <QWidget>

namespace Ui {
class ProfileWidget;
}

class ProfileWidget : public AbstractPage {
    Q_OBJECT

public:
    explicit ProfileWidget(QWidget *parent = nullptr);
    ~ProfileWidget();

protected:
    void SetupPage() override;

private:
    Ui::ProfileWidget *ui;
};

#endif // PROFILEWIDGET_H
