#include "backend.h"
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

Status::Status(bool success, const QString &message)
{
    isSuccess = success;
    response = message;
}

Backend Backend::Instance = Backend();

const QString Backend::BaseUrl = "http://localhost:8080";

Backend::Backend() : myNetworkManager(std::make_unique<QNetworkAccessManager>())
{
    // Temporary
    myToken = "abc121cba";

    connect(myNetworkManager.get(), &QNetworkAccessManager::finished, this, &Backend::OnResponse);
}

QString Backend::GetProjectsUrl()
{
    return BaseUrl + "/project/get/";
}

QString Backend::CreateProjectUrl()
{
    return BaseUrl + "/project/create/";
}

void Backend::Auth(const QString& username, const QString& password)
{
    emit Authenticated(Status(false, "Not implemented"));
}

void Backend::Register(const QString& username, const QString& email, const QString& password)
{
    emit Authenticated(Status(false, "Not implemented"));
}

void Backend::GetProjects()
{
    QUrl url = QUrl(GetProjectsUrl() + "?access_token=" + myToken);
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
}

void Backend::CreateProject(const QString &projectName)
{
    QUrl url = QUrl(CreateProjectUrl() + "?access_token=" + myToken + "&full_name=" + projectName);
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
}

void Backend::GetTasks(const ProjectInfo &projectInfo)
{
    emit TasksLoaded(Status(true, ""), myProjectTasksDictionary[projectInfo]);
}

void Backend::CreateTask(const ProjectInfo &projectInfo, const TaskInfo &taskInfo)
{
    myProjectTasksDictionary[projectInfo].append(taskInfo);
    emit TasksLoaded(Status(true, ""), myProjectTasksDictionary[projectInfo]);
}

void Backend::OnAuth(QNetworkReply *reply)
{

}

void Backend::OnResponse(QNetworkReply* reply)
{
    qInfo() << reply->error();
    QString request = reply->request().url().toString();
    QString pattern = request.sliced(0, request.indexOf('?'));
    if (pattern == GetProjectsUrl()) {
        QNetworkReply::NetworkError error = reply->error();
        bool isSuccess = error == QNetworkReply::NoError;

        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = json.object();

        isSuccess &= root["status"] == "ok";

        QList<ProjectInfo> projects;

        if (isSuccess) {
            for (QJsonValueRef it : root["data"].toArray()) {
                QJsonObject obj = it.toObject();

                QString projName = obj["full_name"].toString();
                int id = obj["id"].toInt();
                int projId = obj["project_id"].toInt();

                projects.push_back(ProjectInfo(id, projId, projName));
            }
        }

        emit ProjectsLoaded(Status(isSuccess, ""), projects);
    } else if (pattern == CreateProjectUrl()) {
        GetProjects();
    }
}

ProjectInfo::ProjectInfo(int id, int projectId, const QString& projectName) : id(id), projectId(projectId), projectName(projectName)
{
}


TaskInfo::TaskInfo(int taskId, int projectId, const QString &taskName, const QString &taskDesc)
    : taskId(taskId), projectId(projectId), taskName(taskName), taskDescription(taskDesc)
{
}
