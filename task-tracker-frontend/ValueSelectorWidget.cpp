#include "Backend.h"
#include "ValueSelectorWidget.h"
#include "ui_ValueSelectorWidget.h"

#include <set>

ValueSelectorWidget::ValueSelectorWidget(QWidget *parent) :
        QWidget(parent), myTask(Context::DEFAULT_TASK), myTaskTag(-1, TagValue(-1, "")), ui(new Ui::ValueSelectorWidget) {
    ui->setupUi(this);

    ui->addDeleteBtn->setText("Add");

    connect(ui->addDeleteBtn, &QAbstractButton::clicked, this, &ValueSelectorWidget::OnAddDeleteClicked);
    connect(ui->propertySelector, &QComboBox::currentIndexChanged, this, &ValueSelectorWidget::OnPropertyChanged);
}

ValueSelectorWidget::~ValueSelectorWidget() {
    delete ui;
}

void ValueSelectorWidget::SetupTags(const QList<TagInfo> &tags, const TaskInfo& taskInfo) {
    myTags = tags;
    myTask = taskInfo;
    isAdded = false;
    ui->propertySelector->clear();
    for (auto& tag : tags) {
        ui->propertySelector->addItem(tag.GetCaption());
    }
    ui->propertySelector->setCurrentIndex(0);
}

void ValueSelectorWidget::SetupTag(const TaskTag &taskTag, bool isEditable) {
    isAdded = true;
    myTaskTag = taskTag;
    ui->addDeleteBtn->setText("Remove");
    if (!isEditable) {
        ui->addDeleteBtn->hide();
    }

    for (int i = 0; i < myTags.count(); i++) {
        QList<TagValue> values = myTags[i].GetValues();
        for (int j = 0; j < values.count(); j++) {
            if (values[j].GetId() == taskTag.GetValue().GetId()) {
                ui->propertySelector->clear();
                ui->propertySelector->addItem(myTags[i].GetCaption());
                ui->valueSelector->clear();
                ui->valueSelector->addItem(taskTag.GetValue().GetValue());

                ui->valueSelector->setEnabled(false);
                ui->propertySelector->setEnabled(false);
                return;
            }
        }
    }
}

void ValueSelectorWidget::OnAddDeleteClicked() {
    if (isAdded) {
        Backend::Instance.RemoveTag(myTaskTag);
        return;
    }

    if (myTags.count() == 0 || myTags[ui->propertySelector->currentIndex()].GetValues().count() == 0) {
        // TODO: handle this
        return;
    }

    TagValue tagValue = myTags[ui->propertySelector->currentIndex()].GetValues()[ui->valueSelector->currentIndex()];

    auto curVals = myTask.GetTags();
    std::set<int> curProps;
    for (auto& val : curVals) {
        curProps.insert(val.GetValue().FindProperty(myTags).GetId());
    }

    if (curProps.count(tagValue.FindProperty(myTags).GetId())) {
        // TODO: handle this
        return;
    }

    Backend::Instance.AddTag(myTask, tagValue);
}

void ValueSelectorWidget::OnPropertyChanged(int idx) {
    ui->valueSelector->clear();
    if (myTags.count() == 0 || idx == -1) {
        return;
    }

    for (auto& tagVal : myTags[idx].GetValues()) {
        ui->valueSelector->addItem(tagVal.GetValue());
    }
}
