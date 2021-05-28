#include "Backend.h"
#include "CommentWidgetItem.h"
#include "ui_CommentWidgetItem.h"

CommentWidgetItem::CommentWidgetItem(QWidget *parent) : QWidget(parent), ui(new Ui::CommentWidgetItem),
        myTask(Context::DEFAULT_TASK), myComment(-1, std::optional<UserInfo>(), QDateTime::currentDateTimeUtc(), "") {
    ui->setupUi(this);

    connect(ui->cancelEditButton, &QAbstractButton::clicked, this, &CommentWidgetItem::OnCancelClicked);
    connect(ui->deleteBtn, &QAbstractButton::clicked, this, &CommentWidgetItem::OnDeleteClicked);
    connect(ui->savePostButton, &QAbstractButton::clicked, this, &CommentWidgetItem::OnSaveClicked);

    connect(&Backend::Instance, &Backend::CommentCreated, this, &CommentWidgetItem::OnCommentPosted);
    connect(&Backend::Instance, &Backend::CommentEdited, this, &CommentWidgetItem::OnCommentEdited);
}

CommentWidgetItem::~CommentWidgetItem() {
    delete ui;
}

void CommentWidgetItem::SetupComment(const CommentInfo &comment) {
    myComment = comment;
    ui->commentEdit->setText(comment.GetText());
    QString header = "Unknown user";
    if (comment.GetCommentator().has_value()) {
        header = comment.GetCommentator().value().GetFullName();
    }

    isNewComment = false;
    ui->savePostButton->setText("Save");
    ui->commentLabel->setText(header);
    ui->infoLabel->setText(comment.GetDate().toString());
    SetEditable(false);
}

void CommentWidgetItem::SetupNewCommentMode(const TaskInfo& task) {
    isNewComment = true;
    ui->deleteBtn->hide();
    ui->cancelEditButton->hide();
    ui->savePostButton->setText("Post");
    ui->commentEdit->setEnabled(true);
    ui->commentEdit->clear();
    ui->commentLabel->setText("Post new comment");
    ui->infoLabel->hide();
    myTask = task;
}

bool CommentWidgetItem::IsNewComment() const {
    return isNewComment;
}

void CommentWidgetItem::SetEditable(bool editable) {
    isEditable = editable;

    if (isEditable) {
        ui->cancelEditButton->setText("Cancel");
        ui->savePostButton->show();
    } else {
        ui->cancelEditButton->setText("Edit");
        ui->savePostButton->hide();
    }

    ui->commentEdit->setEnabled(isEditable);
}

void CommentWidgetItem::OnDeleteClicked() {
    Backend::Instance.DeleteComment(myComment);
}

void CommentWidgetItem::OnSaveClicked() {
    if (isNewComment) {
        CommentInfo newComment(myComment);
        newComment.SetText(ui->commentEdit->toPlainText());
        Backend::Instance.CreateComment(myTask, newComment);
        this->hide();
        return;
    }

    if (isEditable) {
        CommentInfo newComment(myComment);
        newComment.SetText(ui->commentEdit->toPlainText());
        Backend::Instance.EditComment(newComment);
    }
}

void CommentWidgetItem::OnCancelClicked() {
    if (isEditable) {
        SetupComment(myComment);
    }

    SetEditable(!isEditable);
}

void CommentWidgetItem::OnCommentPosted(Status status, const CommentInfo& comment) {
    if (isNewComment && status.isSuccess) {
        if (!status.isSuccess) {
            this->show();
        }
    }
}

void CommentWidgetItem::OnCommentEdited(Status status, const CommentInfo& comment) {
    if (!isNewComment && comment.GetId() == myComment.GetId() && status.isSuccess) {
        myComment.SetText(ui->commentEdit->toPlainText());
        SetupComment(myComment);
    }
}
