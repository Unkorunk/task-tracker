#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore/QObject>
#include <QString>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>
#include <memory>

class Status {
public:
    Status();

    Status(bool success, const QString& message);

    bool isSuccess;
    QString response;
};

class ProjectInfo {
public:
    ProjectInfo(int id, int projectId, const QString& projectName);

    int id;
    int projectId;
    QString projectName;
};

class PropertyValue {
public:
    PropertyValue();

    PropertyValue(int valueId, int propId, QString title);

    int propertyValueId;
    int propertyId;
    QString valueTitle;
};

class PropertyInfo {
public:
    QString caption;
    int propertyId;

    QList<PropertyValue> values;
    int selectedId;
};

class TaskInfo {
public:
    TaskInfo(int taskId, int projectId, const QString& taskName, const QString& taskDesc);

    int taskId;
    int projectId;

    QString taskName;
    QString taskDescription;
    QVector<QString> team;
    QVector<PropertyInfo> properties;
};

class UserInfo {
public:
    UserInfo(const QString& username, const QString& fullName, const QString& email);

    QString GetUsername();
    QString GetFullName();
    QString GetEmail();

private:
    QString myUsername;
    QString myFullName;
    QString myEmail;
};

inline bool operator<(const ProjectInfo &proj1, const ProjectInfo &proj2)
{
    if (proj1.projectId != proj2.projectId)
        return proj1.projectId < proj2.projectId;
    return proj1.projectName < proj2.projectName;
}

class Backend : public QObject {
    Q_OBJECT

public:
    static Backend Instance;

    void SignIn(const QString& username, const QString& password);

    void SignUp(const QString& fullName, const QString& username, const QString& email, const QString& password);

    void GetProjects();

    void CreateProject(const QString& projectName);

    void EditProject(const ProjectInfo& projectInfo);

    void GetTasks(const ProjectInfo& projectInfo);

    void CreateTask(const TaskInfo& taskInfo);

    void DeleteTask(const TaskInfo& taskInfo);

    void EditTask(const TaskInfo& taskInfo);

    UserInfo GetProfile();

    void UpdateProfile();

signals:
    void SignedIn(Status status);

    void SignedUp(Status status);

    void ProjectsLoaded(Status status, const QList<ProjectInfo>& projects);

    void ProjectCreated(Status status);   

    void ProjectEdited(Status status);

    void ProfileUpdated(Status status);

    void TasksLoaded(Status status, const QList<TaskInfo>& tasks);

    void TaskEdited(Status status);

    void TaskDeleted(Status status);

private slots:
    void OnResponse(QNetworkReply* reply);

private:
    static const QString BaseUrl;

    Backend();

    void GetProperties();

    QString GetPropertiesUrl();

    QString GetProjectsUrl();
    QString CreateProjectUrl();
    QString EditProjectUrl();

    QString SignInAccountUrl();
    QString SignUpAccountUrl();
    QString GetAccountUrl();

    QString GetTasksUrl();
    QString CreateTaskUrl();
    QString EditTaskUrl();
    QString DeleteTaskUrl();

    QJsonObject GetRootFromReply(QNetworkReply* reply, Status& errorMsg);

    QMap<int, PropertyValue> valIdToVal;
    QMap<int, QList<PropertyValue>> propIdToVals;
    QMap<int, QString> propIdToCaption;

    QString myToken;
    UserInfo myUserInfo;

    std::unique_ptr<QNetworkAccessManager> myNetworkManager;
};

#endif // BACKEND_H
