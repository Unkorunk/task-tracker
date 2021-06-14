#ifndef DATACLASSES_H
#define DATACLASSES_H

#include <QDateTime>
#include <QList>
#include <QString>


const QString DATE_FORMAT = "yyyy-MM-dd'T'HH:mm:ss.zzz'+00:00'";

class UserInfo {
public:
    static UserInfo ParseFromJson(const QJsonObject& object);

    UserInfo(int id, const QString& fullName, const QString& username, const QString& email);

    int GetId() const;

    QString GetFullName() const;
    void SetFullName(const QString& fullName);

    QString GetUsername() const;
    void SetUsername(const QString& username);

    QString GetEmail() const;
    void SetEmail(const QString& email);

    QString GetPhoto() const;
    void SetPhoto(const QString& photo);

private:
    int myId;
    QString myFullName;
    QString myUsername;
    QString myEmail;
    QString myPhoto;
};

class RoleInfo {
public:
    const static int MANAGE_OWN_TASK = 1;
    const static int MANAGE_ALL_TASK = MANAGE_OWN_TASK | 1 << 1;
    const static int MANAGE_OWN_COMMENT = 1 << 2;
    const static int MANAGE_ALL_COMMENT = MANAGE_OWN_COMMENT | 1 << 3;
    const static int MANAGE_TEAM = 1 << 4;
    const static int MANAGE_ROLES = 1 << 5;
    const static int MANAGE_PROJECT = 1 << 6;
    const static int DELETE_PROJECT = 1 << 7;

    static RoleInfo ParseFromJson(const QJsonObject& obj);

    RoleInfo(int id, const QString& caption, uint64_t perms, int projectId);

    int GetId() const;
    QString GetCaption() const;
    void SetCaption(const QString& caption);

    uint64_t GetPermission() const;
    QString GetPermissionStr() const;
    void SetPermissions(uint64_t perm, bool add = true);

    int GetProjectId() const;

    bool HasPermission(uint64_t perm) const;

private:
    int myId;
    QString myCaption;
    uint64_t myPermissions;

    int myProjectId;
};

class ProjectInfo {
public:
    static ProjectInfo ParseFromJson(const QJsonObject& object);

    ProjectInfo(int id, const QString& title);

    int GetId() const;

    QString GetTitle() const;
    void SetTitle(const QString& title);

    QString GetIcon() const;
    void SetIcon(const QString& icon);

private:
    int myId;
    QString myTitle;
    QString myIcon;
};

class CommentInfo {
public:
    static CommentInfo ParseFromJson(const QJsonObject& object);

    CommentInfo(int id, const std::optional<UserInfo>& user, const QDateTime& date, const QString& text);

    int GetId() const;

    std::optional<UserInfo> GetCommentator() const;
    void SetCommentator(const std::optional<UserInfo>& user, const QDateTime& creationDate);

    QDateTime GetDate() const;

    QString GetText() const;
    void SetText(const QString& text);

private:
    int myId;
    std::optional<UserInfo> myUser;
    QDateTime myDate;
    QString myText;
};

class TagInfo;

class TagValue {
public:
    static TagValue ParseFromJson(const QJsonObject& obj);

    TagValue(int id, const QString& value);

    int GetId() const;

    QString GetValue() const;
    void SetValue(const QString& value);

    TagInfo FindProperty(const QList<TagInfo>& properties);

private:
    int myId;
    QString myValue;
};

class TagInfo {
public:
    static TagInfo ParseFromJson(const QJsonObject& obj);

    TagInfo(int id, int projectId, const QString& caption, const QList<TagValue>& values);

    int GetId() const;

    int GetProjectId() const;

    QString GetCaption() const;
    void SetCaption(const QString& caption);

    QList<TagValue> GetValues() const;
    void SetValues(const QList<TagValue>& values);

private:
    int myId;
    int myProjectId;
    QString myCaption;
    QList<TagValue> myValues;
};

class TaskTag {
public:
    static TaskTag ParseFromJson(const QJsonObject& obj);

    TaskTag(int id, const TagValue& tagValue);

    int GetId() const;

    TagValue GetValue() const;
    void SetValue(const TagValue& tag);

private:
    int myId;
    TagValue myValue;
};

class NotificationInfo {
public:
    static NotificationInfo ParseFromJson(const QJsonObject& obj);

    NotificationInfo(int id, QString text);

    int GetId();

    void SetText(QString text);
    QString GetText() const;

private:
    int myId;
    QString myText;
};

class TaskInfo {
public:
    static TaskInfo ParseFromJson(const QJsonObject& object);

    TaskInfo(int id, const ProjectInfo& project, const QString& title, const QString& description, const std::optional<UserInfo>& creator,
             const QDateTime& creationTime, int storyPoints);

    int GetId() const;

    const ProjectInfo& GetProject() const;
    int GetProjectId() const;

    QString GetTitle() const;
    void SetTitle(const QString& title);

    QString GetDescription() const;
    void SetDescription(const QString& description);

    std::optional<UserInfo> GetCreator() const;
    QDateTime GetCreationTime() const;

    std::optional<UserInfo> GetUpdater() const;
    QDateTime GetUpdateTime() const;
    void SetUpdater(const std::optional<UserInfo>& updater, const QDateTime& updateTime);

    std::optional<UserInfo> GetAssignee() const;
    void SetAssignee(const std::optional<UserInfo>& assignee);

    int GetStoryPoints() const;
    void SetStoryPoints(int storyPoints);

    std::optional<QDateTime> GetDeadline() const;
    void SetDeadline(const std::optional<QDateTime>& deadline);

    QList<CommentInfo> GetComments() const;
    void SetComments(const QList<CommentInfo>& comments);

    QList<TaskTag> GetTags() const;
    void SetTags(const QList<TaskTag>& tags);

private:
    int myId;
    ProjectInfo myProject;

    QString myTitle;
    QString myDescription;

    std::optional<UserInfo> myCreator;
    QDateTime myCreationTime;

    std::optional<UserInfo> myUpdater;
    QDateTime myUpdateTime;

    std::optional<UserInfo> myAssignee;

    int myStoryPoints;

    //QVector<PropertyInfo> myProperties;

    std::optional<QDateTime> myDeadline;

    QList<CommentInfo> myComments;
    QList<TaskTag> myTags;
};

inline bool operator<(const ProjectInfo &proj1, const ProjectInfo &proj2)
{
    if (proj1.GetId() != proj2.GetId())
        return proj1.GetId() < proj2.GetId();
    return proj1.GetTitle() < proj2.GetTitle();
}

#endif // DATACLASSES_H
