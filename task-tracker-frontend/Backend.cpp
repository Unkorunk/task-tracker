#include "Backend.h"
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMetaEnum>
#include <QUrlQuery>

Status::Status() : Status(true, "") {}

Status::Status(bool success, const QString &message)
{
    isSuccess = success;
    response = message;
}

Backend Backend::Instance = Backend();

const QString Backend::BaseUrl = "http://localhost:8080";

Backend::Backend() : myUserInfo("", "", ""), myNetworkManager(std::make_unique<QNetworkAccessManager>())
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

QString Backend::EditProjectUrl()
{
    return BaseUrl + "/project/edit/";
}

QString Backend::SignInAccountUrl()
{
    return BaseUrl + "/account/enter";
}

QString Backend::SignUpAccountUrl()
{
    return BaseUrl + "/account/create";
}

QString Backend::GetAccountUrl()
{
    return BaseUrl + "/account/get";
}

QString Backend::GetTasksUrl()
{
    return BaseUrl + "/task/get";
}

QJsonObject Backend::GetRootFromReply(QNetworkReply *reply, Status &status)
{
    QNetworkReply::NetworkError error = reply->error();

    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = json.object();

    status.response = QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(error);
    status.response += " :: " + root["status"].toString() + " >> " + root["description"].toString();

    status.isSuccess = (error == QNetworkReply::NoError) && (root["status"] == "ok");

    return root;
}

void Backend::SignIn(const QString& username, const QString& password)
{
    QUrl url = QUrl(SignInAccountUrl() + "?username=" + username + "&password=" + password);
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
}

void Backend::SignUp(const QString& fullName, const QString& username, const QString& email, const QString& password)
{
    QUrl url = QUrl(SignUpAccountUrl() + "?full_name=" + fullName+ "&username=" + username + "&email=" + email + "&password=" + password);
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
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

void Backend::EditProject(const ProjectInfo& projectInfo)
{
    QUrlQuery query;

    query.addQueryItem("project_id", QString("%1").arg(projectInfo.projectId));
    query.addQueryItem("access_token", myToken);
    query.addQueryItem("fields", QString("{\"full_name\":\"%1\"}").arg(projectInfo.projectName));

    QUrl url = QUrl(EditProjectUrl());
    url.setQuery(query.toString(QUrl::FullyEncoded).toUtf8());
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    myNetworkManager->post(request, "");
    qInfo() << request.url();
};

void Backend::GetTasks(const ProjectInfo &projectInfo)
{
    QUrl url = QUrl(GetTasksUrl() + QString("?project_id=%1").arg(projectInfo.projectId));
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
}

void Backend::CreateTask(const ProjectInfo &projectInfo, const TaskInfo &taskInfo)
{
    myProjectTasksDictionary[projectInfo].append(taskInfo);
    emit TasksLoaded(Status(true, ""), myProjectTasksDictionary[projectInfo]);
}

void Backend::UpdateTask(const ProjectInfo& projectInfo, const TaskInfo &taskInfo)
{
    myProjectTasksDictionary[projectInfo].at(taskInfo.taskId);
    //emit TasksLoaded(Status(true, ""), myProjectTasksDictionary[projectInfo]);
}

UserInfo Backend::GetProfile()
{
    return myUserInfo;
}

void Backend::UpdateProfile()
{
    QUrl url = QUrl(GetAccountUrl() + "?access_token=" + myToken);
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
}

void Backend::OnResponse(QNetworkReply* reply)
{
    Status status;
    QJsonObject root = GetRootFromReply(reply, status);

    qInfo() << status.response;
    qInfo() << reply->url();

    QString request = reply->request().url().toString();
    if (request.indexOf('?') == -1) {
        return;
    }

    QString pattern = request.sliced(0, request.indexOf('?'));

    if (pattern == GetProjectsUrl()) {
        QList<ProjectInfo> projects;

        if (status.isSuccess) {
            for (QJsonValueRef it : root["data"].toArray()) {
                QJsonObject obj = it.toObject();

                QString projName = obj["full_name"].toString();
                int id = obj["id"].toInt();
                int projId = obj["project_id"].toInt();

                projects.push_back(ProjectInfo(id, projId, projName));
            }
        }

        emit ProjectsLoaded(status, projects);
    } else if (pattern == CreateProjectUrl()) {
        GetProjects();

        emit ProjectCreated(status);
    } else if (pattern == EditProjectUrl()) {
        if (status.isSuccess) {
            GetProjects();
        }

        emit ProjectEdited(status);
    } else if (pattern == SignInAccountUrl()) {
        if (status.isSuccess) {
            myToken = root["data"].toObject()["access_token"].toString();
        }

        emit SignedIn(status);
    } else if (pattern == SignUpAccountUrl()) {
        if (status.isSuccess) {
            myToken = root["data"].toObject()["access_token"].toString();
        }

        emit SignedUp(status);
    } else if (pattern == GetAccountUrl()) {
        if (status.isSuccess) {
            QJsonObject data = root["data"].toObject();
            myUserInfo = UserInfo(data["username"].toString(), data["full_name"].toString(), data["email"].toString());
        }

        emit ProfileUpdated(status);
    } else if (pattern == GetTasksUrl()) {

        emit TasksLoaded(status, myProjectTasksDictionary[projectInfo]);
    }
}

ProjectInfo::ProjectInfo(int id, int projectId, const QString& projectName) : id(id), projectId(projectId), projectName(projectName)
{
}


TaskInfo::TaskInfo(int taskId, int projectId, const QString &taskName, const QString &taskDesc)
    : taskId(taskId), projectId(projectId), taskName(taskName), taskDescription(taskDesc)
{
}

UserInfo::UserInfo(const QString &username, const QString &fullName, const QString &email)
    : myUsername(username), myFullName(fullName), myEmail(email)
{
}

QString UserInfo::GetUsername()
{
    return myUsername;
}

QString UserInfo::GetFullName()
{
    return myFullName;
}

QString UserInfo::GetEmail()
{
    return myEmail;
}
