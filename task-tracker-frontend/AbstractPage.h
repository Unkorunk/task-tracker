#ifndef ABSTRACTPAGE_H
#define ABSTRACTPAGE_H

#include "Context.h"

#include <QWidget>

class AbstractPage : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractPage(QWidget *parent = nullptr);

    void Setup(const Context& context);

protected:
    virtual void SetupPage() = 0;

    Context myContext;
};

#endif // ABSTRACTPAGE_H
