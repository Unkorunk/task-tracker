#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore/QObject>
#include <QString>
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
    ProjectInfo(int id, int projectId, QString projectName);

    int id;
    int projectId;
    QString projectName;
};

class Backend : public QObject {
    Q_OBJECT

public:
    static Backend Instance;

    void Auth(const QString& username, const QString& password);

    void Register(const QString& username, const QString& email, const QString& password);

    void GetProjects();

    void CreateProject(const QString& projectName);

signals:
    void Authenticated(Status status);

    void ProjectsLoaded(Status status, const QList<ProjectInfo>& projects);
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
};

#endif // BACKEND_H
