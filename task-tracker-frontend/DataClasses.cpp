#include "DataClasses.h"

#include <QJsonObject>

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

// END TASK INFO

// ROLE INFO
RoleInfo RoleInfo::ParseFromJson(const QJsonObject &obj) {
    QByteArray perms = QByteArray::fromStdString(obj["permissions"].toString().toStdString());
    return RoleInfo(obj["id"].toInt(), obj["value"].toString(), perms, obj["project"].toObject()["id"].toInt());
}

RoleInfo::RoleInfo(int id, const QString &caption, QByteArray perms, int projectId) : myId(id), myCaption(caption),
    myPermissions(perms), myProjectId(projectId) {}

int RoleInfo::GetId() const {
    return myId;
}

QString RoleInfo::GetCaption() const {
    return myCaption;
}

QByteArray RoleInfo::GetPermission() const {
    return myPermissions;
}

int RoleInfo::GetProjectId() const {
    return myProjectId;
}

// END ROLE INFO
