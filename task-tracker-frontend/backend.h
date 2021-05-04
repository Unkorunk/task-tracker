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

class Backend : public QObject {
    Q_OBJECT

public:
    static Backend Instance;

    void Auth(QString username, QString password);

    void Register(QString username, QString email, QString password);

    void GetProjects();

signals:
    void Authenticated(Status status);

    void ProjectsLoaded(Status status);
private slots:
    void OnAuth(QNetworkReply* reply);

    void OnProjectsLoaded(QNetworkReply* reply);

private:
    Backend();

    static const QString BaseUrl;

    QString myToken;

    QNetworkAccessManager* myNetworkManager;
};

#endif // BACKEND_H
