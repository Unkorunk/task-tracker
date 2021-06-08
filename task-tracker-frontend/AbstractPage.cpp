#include "AbstractPage.h"

AbstractPage::AbstractPage(QWidget *parent) : QWidget(parent) { }

void AbstractPage::Setup(const Context &context) {
    myContext = context;
    SetupPage();
}
