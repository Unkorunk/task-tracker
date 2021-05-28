#ifndef COMMENTWIDGETITEM_H
#define COMMENTWIDGETITEM_H

#include "Backend.h"
#include "DataClasses.h"

#include <QWidget>

namespace Ui {
class CommentWidgetItem;
}

class CommentWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit CommentWidgetItem(QWidget *parent = nullptr);
    ~CommentWidgetItem();

    void SetupComment(const CommentInfo& comment);

    void SetupNewCommentMode(const TaskInfo& taskId);
    bool IsNewComment() const;

    bool GetEditable() const;

private slots:
    void OnDeleteClicked();
    void OnSaveClicked();
    void OnCancelClicked();

    void OnCommentPosted(Status status, const CommentInfo& comment);
    void OnCommentEdited(Status status, const CommentInfo& comment);

private:
    void SetEditable(bool editable);

    Ui::CommentWidgetItem *ui;
    TaskInfo myTask;
    CommentInfo myComment;

    bool isNewComment;
    bool isEditable;
};

#endif // COMMENTWIDGETITEM_H
