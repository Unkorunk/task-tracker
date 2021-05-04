#include "backend.h"
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Status::Status(bool success, const QString &message)
{
    isSuccess = success;
    response = message;
}

Backend Backend::Instance = Backend();

const QString Backend::BaseUrl = "http://localhost:8080";

Backend::Backend() : myNetworkManager(new QNetworkAccessManager())
{
    // Temporary
    myToken = "abc121cba";

    connect(myNetworkManager, &QNetworkAccessManager::finished, this, &Backend::OnProjectsLoaded);
}

void Backend::Auth(QString username, QString password)
{
    emit Authenticated(Status(false, "Not implemented"));
}

void Backend::Register(QString username, QString email, QString password)
{
    emit Authenticated(Status(false, "Not implemented"));
}

void Backend::GetProjects()
{
    myNetworkManager->get(QNetworkRequest(QUrl(BaseUrl + "/project/get/?access_token=" + myToken)));
}

void Backend::OnAuth(QNetworkReply *reply)
{

}

void Backend::OnProjectsLoaded(QNetworkReply* reply)
{
    QNetworkReply::NetworkError error = reply->error();
    bool isError = error != QNetworkReply::NoError;

    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = json.object();

    isError |= root["status"] != "ok";

    if (!isError) {
        for (QJsonValueRef it : root["data"].toArray()) {
            QJsonObject obj = it.toObject();
            QString projName = obj["full_name"].toString();
            int id = obj["id"].toInt();
            int projId = obj["project_id"].toInt();

        }
    }

    emit ProjectsLoaded(Status(isError, ""));
}
