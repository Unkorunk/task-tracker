#include "Backend.h"
#include "PropertyItemWidget.h"
#include "PropertyValueItemWidget.h"
#include "ui_PropertyItemWidget.h"

#include <QListWidget>

PropertyItemWidget::PropertyItemWidget(QWidget *parent) :
        QWidget(parent), myTag(-1, -1, "", QList<TagValue>()), ui(new Ui::PropertyItemWidget) {
    ui->setupUi(this);
    SetEditable(false);

    connect(ui->deleteBtn, &QAbstractButton::clicked, this, &PropertyItemWidget::OnDeleteBtnClicked);
    connect(ui->saveCaptionBtn, &QAbstractButton::clicked, this, &PropertyItemWidget::OnSaveBtnClicked);
    connect(ui->editCancelBtn, &QAbstractButton::clicked, this, &PropertyItemWidget::OnEditCancelBtnClicked);
    connect(ui->newValueBtn, &QAbstractButton::clicked, this, &PropertyItemWidget::OnAddValueClicked);
}

PropertyItemWidget::~PropertyItemWidget() {
    delete ui;
}

void PropertyItemWidget::SetupTag(const TagInfo &tag) {
    SetEditable(false);

    myTag = tag;
    ui->captionEdit->setText(tag.GetCaption());
    ui->valuesList->clear();

    for (auto& tagValue : myTag.GetValues()) {
        auto item = new QListWidgetItem();
        auto widget = new PropertyValueItemWidget(this);
        widget->SetupValue(tagValue);
        item->setSizeHint(QSize(200, 40));
        ui->valuesList->addItem(item);
        ui->valuesList->setItemWidget(item, widget);
    }
}

TagInfo PropertyItemWidget::GetTag() const {
    return myTag;
}

void PropertyItemWidget::OnDeleteBtnClicked() {
    Backend::Instance.DeleteTagCaption(myTag);
}

void PropertyItemWidget::OnSaveBtnClicked() {
    TagInfo newTag = TagInfo(myTag);
    newTag.SetCaption(ui->captionEdit->text());
    Backend::Instance.EditTagCaption(newTag);
}

void PropertyItemWidget::OnEditCancelBtnClicked() {
    if (isEditable) {
        SetupTag(myTag);
    }

    SetEditable(!isEditable);
}

void PropertyItemWidget::OnAddValueClicked() {
    Backend::Instance.CreateTagValue(myTag, ui->valueEdit->text());
}

void PropertyItemWidget::SetEditable(bool editable) {
    isEditable = editable;
    ui->captionEdit->setEnabled(editable);
    if (isEditable) {
        ui->saveCaptionBtn->show();
        ui->editCancelBtn->setText("Cancel");
    } else {
        ui->saveCaptionBtn->hide();
        ui->editCancelBtn->setText("Edit");
    }
}
