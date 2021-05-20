#ifndef DATACLASSES_H
#define DATACLASSES_H

#include <QDateTime>
#include <QList>
#include <QString>


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

// TODO: add properties
//class PropertyValue {
//public:
//    PropertyValue();

//    PropertyValue(int valueId, int propId, QString title);

//    int propertyValueId;
//    int propertyId;
//    QString valueTitle;
//};

//class PropertyInfo {
//public:
//    QString caption;
//    int propertyId;

//    QList<PropertyValue> values;
//    int selectedId;
//};

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

};

inline bool operator<(const ProjectInfo &proj1, const ProjectInfo &proj2)
{
    if (proj1.GetId() != proj2.GetId())
        return proj1.GetId() < proj2.GetId();
    return proj1.GetTitle() < proj2.GetTitle();
}

#endif // DATACLASSES_H
