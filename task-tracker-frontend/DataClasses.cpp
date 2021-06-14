#include "DataClasses.h"

#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

// PROJECT INFO
ProjectInfo ProjectInfo::ParseFromJson(const QJsonObject &object) {
    ProjectInfo project(object["id"].toInt(), object["fullName"].toString());
    if (object.contains("photo") && !object["photo"].isNull()) {
        project.SetIcon(object["photo"].toString());
    }

    return project;
}

ProjectInfo::ProjectInfo(int id, const QString &title)
    : myId(id), myTitle(title) {}

int ProjectInfo::GetId() const {
    return myId;
}

QString ProjectInfo::GetTitle() const {
    return myTitle;
}

void ProjectInfo::SetTitle(const QString &title) {
    myTitle = title;
}

QString ProjectInfo::GetIcon() const {
    return myIcon;
}

void ProjectInfo::SetIcon(const QString &icon) {
    myIcon = icon;
}

// END PROJECT INFO

// USER INFO
UserInfo UserInfo::ParseFromJson(const QJsonObject &object) {
    UserInfo userInfo(object["id"].toInt(), object["fullName"].toString(), object["screenName"].toString(), object["email"].toString());
    if (object.contains("photo") && !object["photo"].isNull()) {
        userInfo.SetPhoto(object["photo"].toString());
    }

    return userInfo;
}

UserInfo::UserInfo(int id, const QString &fullName, const QString &username, const QString &email)
    : myId(id), myFullName(fullName), myUsername(username), myEmail(email) {}

int UserInfo::GetId() const {
    return myId;
}

QString UserInfo::GetFullName() const {
    return myFullName;
}

void UserInfo::SetFullName(const QString &fullName) {
    myFullName = fullName;
}

QString UserInfo::GetUsername() const {
    return myUsername;
}

void UserInfo::SetUsername(const QString &username) {
    myUsername = username;
}

QString UserInfo::GetEmail() const {
    return myEmail;
}

void UserInfo::SetEmail(const QString &email) {
    myEmail = email;
}

QString UserInfo::GetPhoto() const {
    return myPhoto;
}

void UserInfo::SetPhoto(const QString &photo) {
    myPhoto = photo;
}

// END USER INFO

// COMMENT INFO
CommentInfo CommentInfo::ParseFromJson(const QJsonObject &object) {
    std::optional<UserInfo> user;
    if (object.contains("author") && !object["author"].isNull()) {
        user.emplace(UserInfo::ParseFromJson(object["author"].toObject()));
    }

    CommentInfo comment(object["id"].toInt(), user, QDateTime::fromString(object["createdAt"].toString(), DATE_FORMAT),
            object["text"].toString());
    return comment;
}

CommentInfo::CommentInfo(int id, const std::optional<UserInfo> &user, const QDateTime& date, const QString &text)
    : myId(id), myUser(user), myDate(date), myText(text) { }

int CommentInfo::GetId() const {
    return myId;
}

std::optional<UserInfo> CommentInfo::GetCommentator() const {
    return myUser;
}

void CommentInfo::SetCommentator(const std::optional<UserInfo> &user, const QDateTime& creationDate) {
    myUser = user;
    myDate = creationDate;
}

QDateTime CommentInfo::GetDate() const {
    return myDate;
}

QString CommentInfo::GetText() const {
    return myText;
}

void CommentInfo::SetText(const QString &text) {
    myText = text;
}

// END COMMENT INFO

// TASK INFO
TaskInfo TaskInfo::ParseFromJson(const QJsonObject &object) {
    std::optional<UserInfo> creator;
    if (object.contains("createdBy") && !object["createdBy"].isNull()) {
        creator.emplace(UserInfo::ParseFromJson(object["createdBy"].toObject()));
    }

    TaskInfo task(object["id"].toInt(), ProjectInfo::ParseFromJson(object["project"].toObject()), object["title"].toString(),
            object["description"].toString(), creator, QDateTime::fromString(object["createdAt"].toString(), DATE_FORMAT), object["storyPoints"].toInt());

    std::optional<UserInfo> updater;
    if (object.contains("updatedBy") && !object["updatedBy"].isNull()) {
        updater.emplace(UserInfo::ParseFromJson(object["updatedBy"].toObject()));
    }

    task.SetUpdater(updater, QDateTime::fromString(object["updatedAt"].toString(), DATE_FORMAT));

    if (object.contains("assignedTo") && !object["assignedTo"].isNull()) {
        task.SetAssignee(UserInfo::ParseFromJson(object["assignedTo"].toObject()));
    }

    if (object.contains("deadline") && !object["deadline"].isNull()) {
        task.SetDeadline(QDateTime::fromString(object["deadline"].toString(), DATE_FORMAT));
    }

    QList<CommentInfo> comments;
    for (QJsonValueRef comment : object["comments"].toArray()) {
        comments.push_back(CommentInfo::ParseFromJson(comment.toObject()));
    }
    task.SetComments(comments);

    QList<TaskTag> tags;
    for (QJsonValueRef tag : object["tags"].toArray()) {
        tags.push_back(TaskTag::ParseFromJson(tag.toObject()));
    }
    task.SetTags(tags);

    return task;
}

TaskInfo::TaskInfo(int id, const ProjectInfo &project, const QString &title, const QString &description,
                   const std::optional<UserInfo> &creator, const QDateTime &creationTime, int storyPoints)
    : myId(id), myProject(project), myTitle(title), myDescription(description), myCreator(creator), myCreationTime(creationTime),
      myUpdater(creator), myUpdateTime(creationTime), myStoryPoints(storyPoints) {}

int TaskInfo::GetId() const {
    return myId;
}

const ProjectInfo &TaskInfo::GetProject() const {
    return myProject;
}

int TaskInfo::GetProjectId() const {
    return myProject.GetId();
}

QString TaskInfo::GetTitle() const {
    return myTitle;
}

void TaskInfo::SetTitle(const QString &title) {
    myTitle = title;
}

QString TaskInfo::GetDescription() const {
    return myDescription;
}

void TaskInfo::SetDescription(const QString &description) {
    myDescription = description;
}

std::optional<UserInfo> TaskInfo::GetCreator() const {
    return myCreator;
}

QDateTime TaskInfo::GetCreationTime() const {
    return myCreationTime;
}

std::optional<UserInfo> TaskInfo::GetUpdater() const {
    return myUpdater;
}

QDateTime TaskInfo::GetUpdateTime() const {
    return myUpdateTime;
}

void TaskInfo::SetUpdater(const std::optional<UserInfo>& updater, const QDateTime &updateTime) {
    myUpdater = updater;
    myUpdateTime = updateTime;
}

std::optional<UserInfo> TaskInfo::GetAssignee() const {
    return myAssignee;
}

void TaskInfo::SetAssignee(const std::optional<UserInfo> &assignee) {
    myAssignee = assignee;
}

int TaskInfo::GetStoryPoints() const {
    return myStoryPoints;
}

void TaskInfo::SetStoryPoints(int storyPoints) {
    myStoryPoints = storyPoints;
}

std::optional<QDateTime> TaskInfo::GetDeadline() const {
    return myDeadline;
}

void TaskInfo::SetDeadline(const std::optional<QDateTime> &deadline) {
    myDeadline = deadline;
}

QList<CommentInfo> TaskInfo::GetComments() const {
    return myComments;
}

void TaskInfo::SetComments(const QList<CommentInfo> &comments) {
    myComments = comments;
}

QList<TaskTag> TaskInfo::GetTags() const {
    return myTags;
}

void TaskInfo::SetTags(const QList<TaskTag> &tags) {
    myTags = tags;
}

// END TASK INFO

// ROLE INFO
RoleInfo RoleInfo::ParseFromJson(const QJsonObject &obj) {
    QString bitString = obj["permissions"].toString();
    uint64_t perms = 0;
    for (uint64_t i = 0; i < std::min(64ll, bitString.length()); i++) {
        if (bitString[i] == '1') {
            perms += (1ull << i);
        }
    }

    return RoleInfo(obj["id"].toInt(), obj["value"].toString(), perms, obj["project"].toObject()["id"].toInt());
}

RoleInfo::RoleInfo(int id, const QString &caption, uint64_t perms, int projectId) : myId(id), myCaption(caption),
    myPermissions(perms), myProjectId(projectId) {}

int RoleInfo::GetId() const {
    return myId;
}

QString RoleInfo::GetCaption() const {
    return myCaption;
}

void RoleInfo::SetCaption(const QString &caption) {
    myCaption = caption;
}

uint64_t RoleInfo::GetPermission() const {
    return myPermissions;
}

QString RoleInfo::GetPermissionStr() const {
    QString res;
    uint64_t mask = 0;
    for (uint64_t i = 0; i < 8; i++) {
        mask = 255 << (i * 8ull);
        res.append((char)((myPermissions & mask) >> (i * 8ull)));
    }

    return res;
}

void RoleInfo::SetPermissions(uint64_t perm, bool add) {
    if (add) {
        myPermissions |= perm;
    }
}

int RoleInfo::GetProjectId() const {
    return myProjectId;
}

bool RoleInfo::HasPermission(uint64_t perm) const {
    return ((myPermissions & perm) == perm);
}

// END ROLE INFO

// TAG VALUE
TagValue TagValue::ParseFromJson(const QJsonObject &obj) {
    return TagValue(obj["id"].toInt(), obj["value"].toString());
}

TagValue::TagValue(int id, const QString &value)
    : myId(id), myValue(value) {}

int TagValue::GetId() const {
    return myId;
}

QString TagValue::GetValue() const {
    return myValue;
}

void TagValue::SetValue(const QString &value) {
    myValue = value;
}

TagInfo TagValue::FindProperty(const QList<TagInfo> &properties) {
    for (int i = 0; i < properties.count(); i++) {
        QList<TagValue> values = properties[i].GetValues();
        for (int j = 0; j < values.count(); j++) {
            if (values[j].GetId() == myId) {
                return properties[i];
            }
        }
    }

    return TagInfo(-1, -1, "", QList<TagValue>());
}

// END TAG VALUE

// TAG INFO
TagInfo TagInfo::ParseFromJson(const QJsonObject &obj) {
    QList<TagValue> values;
    for (QJsonValueRef it : obj["values"].toArray()) {
        values.push_back(TagValue::ParseFromJson(it.toObject()));
    }

    return TagInfo(obj["id"].toInt(), obj["project"].toObject()["id"].toInt(), obj["caption"].toString(), values);
}

TagInfo::TagInfo(int id, int projectId, const QString &caption, const QList<TagValue>& values)
    : myId(id), myProjectId(projectId), myCaption(caption), myValues(values) {}

int TagInfo::GetId() const {
    return myId;
}

int TagInfo::GetProjectId() const {
    return myProjectId;
}

QString TagInfo::GetCaption() const {
    return myCaption;
}

void TagInfo::SetCaption(const QString &caption) {
    myCaption = caption;
}

QList<TagValue> TagInfo::GetValues() const {
    return myValues;
}

void TagInfo::SetValues(const QList<TagValue> &values) {
    myValues = values;
}

// END TAG INFO

// TASK TAG
TaskTag TaskTag::ParseFromJson(const QJsonObject &obj) {
    TagValue val = TagValue::ParseFromJson(obj["tagValue"].toObject());
    return TaskTag(obj["id"].toInt(), val);
}

TaskTag::TaskTag(int id, const TagValue &tagValue) : myId(id), myValue(tagValue) {}

int TaskTag::GetId() const {
    return myId;
}


TagValue TaskTag::GetValue() const {
    return myValue;
}

void TaskTag::SetValue(const TagValue &tag) {
    myValue = tag;
}

int NotificationInfo::GetId() {
    return myId;
}

QString NotificationInfo::GetText() const {
    return myText;
}

void NotificationInfo::SetText(QString text) {
    myText = text;
}

NotificationInfo::NotificationInfo(int id, QString text): myId(id), myText(text) {

}

NotificationInfo NotificationInfo::ParseFromJson(const QJsonObject &obj) {
    return NotificationInfo(obj["id"].toInt(), obj["text"].toString());
}

// END TASK TAG
