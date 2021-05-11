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

class TaskInfo {
public:
    TaskInfo(int taskId, int projectId, const QString& taskName, const QString& taskDesc);

    int taskId;
    int projectId;

    QString taskName;
    QString taskDescription;
    QVector<QString> team;
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

    void UpdateProject(const ProjectInfo& projectInfo);

    void GetTasks(const ProjectInfo& projectInfo);

    void CreateTask(const ProjectInfo& projectInfo, const TaskInfo& taskInfo);

    void UpdateTask(const ProjectInfo& projectInfo, const TaskInfo& taskInfo);

    UserInfo GetProfile();

    void UpdateProfile();

signals:
    void SignedIn(Status status);

    void SignedUp(Status status);

    void ProjectsLoaded(Status status, const QList<ProjectInfo>& projects);

    void ProjectCreated(Status status);   

    void TasksLoaded(Status status, const QList<TaskInfo>& tasks);

    void ProfileUpdated(Status status);

private slots:
    void OnResponse(QNetworkReply* reply);

private:
    static const QString BaseUrl;

    Backend();

    QString GetProjectsUrl();
    QString CreateProjectUrl();
    QString SignInAccountUrl();
    QString SignUpAccountUrl();
    QString GetAccountUrl();

    QJsonObject GetRootFromReply(QNetworkReply* reply, Status& errorMsg);

    QString myToken;
    UserInfo myUserInfo;

    std::unique_ptr<QNetworkAccessManager> myNetworkManager;

    QMap<ProjectInfo, QList<TaskInfo>> myProjectTasksDictionary;
};

#endif // BACKEND_H
