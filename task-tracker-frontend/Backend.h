#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore/QObject>
#include <QString>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>
#include <memory>

class Status {
public:
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

    void Auth(const QString& username, const QString& password);

    void Register(const QString& username, const QString& email, const QString& password);

    void GetProjects();

    void CreateProject(const QString& projectName);

    void GetTasks(const ProjectInfo& projectInfo);

    void CreateTask(const ProjectInfo& projectInfo, const TaskInfo& taskInfo);

    void UpdateTask(const ProjectInfo& projectInfo, const TaskInfo& taskInfo);

signals:
    void Authenticated(Status status);

    void ProjectsLoaded(Status status, const QList<ProjectInfo>& projects);

    void TasksLoaded(Status status, const QList<TaskInfo>& tasks);

private slots:
    void OnAuth(QNetworkReply* reply);

    void OnResponse(QNetworkReply* reply);

private:
    Backend();

    QString GetProjectsUrl();
    QString CreateProjectUrl();

    static const QString BaseUrl;

    QString myToken;

    std::unique_ptr<QNetworkAccessManager> myNetworkManager;

    QMap<ProjectInfo, QList<TaskInfo>> myProjectTasksDictionary;
};

#endif // BACKEND_H
