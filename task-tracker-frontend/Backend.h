#ifndef BACKEND_H
#define BACKEND_H

#include "DataClasses.h"

#include <QtCore/QObject>
#include <QString>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>

class Status {
public:
    Status();

    Status(bool success, const QString& message);

    bool isSuccess;
    QString response;
};

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

    void GetRoles(const ProjectInfo& projectInfo);
    void CreateRole(const RoleInfo& roleInfo);
    void EditRole(const RoleInfo& roleInfo);
    void DeleteRole(const RoleInfo& roleInfo);

signals:
    void SignedIn(Status status);
    void SignedUp(Status status);

    void ProjectsLoaded(Status status, const QList<ProjectInfo>& projects);
    void ProjectCreated(Status status);   
    void ProjectEdited(Status status);

    void ProfileUpdated(Status status);

    void TasksLoaded(Status status, const QList<TaskInfo>& tasks);
    void TaskEdited(Status status, const TaskInfo& task);
    void TaskDeleted(Status status);

    void RolesLoaded(Status status, const QList<RoleInfo>& roles);
    void RoleCreated(Status statsu, const RoleInfo& role);
    void RoleEdited(Status statsu, const RoleInfo& role);
    void RoleDeleted(Status status);

private slots:
    void OnResponse(QNetworkReply* reply);

private:
    static const QString BaseUrl;

    Backend();

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

    QString GetRolesUrl();
    QString CreateRoleUrl();
    QString EditRoleUrl();
    QString DeleteRoleUrl();

    QJsonObject GetRootFromReply(QNetworkReply* reply, Status& errorMsg);

    void PostRequest(const QString& urlString, const QMap<QString, QString>& args);

    void GetRequest(const QString& urlString, const QMap<QString, QString>& args);

//    QMap<int, PropertyValue> valIdToVal;
//    QMap<int, QList<PropertyValue>> propIdToVals;
//    QMap<int, QString> propIdToCaption;

    QString myToken;
    UserInfo myUserInfo;

    std::unique_ptr<QNetworkAccessManager> myNetworkManager;
};

#endif // BACKEND_H
