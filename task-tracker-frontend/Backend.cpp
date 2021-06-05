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

Status::Status(bool success, const QString &message) {
    isSuccess = success;
    response = message;
}

Backend Backend::Instance = Backend();

const QString Backend::BaseUrl = "http://139.59.144.121:8080";

Backend::Backend() : myUserInfo(-1, "", "", ""), myNetworkManager(std::make_unique<QNetworkAccessManager>()) {
    myToken = "";
    connect(myNetworkManager.get(), &QNetworkAccessManager::finished, this, &Backend::OnResponse);
}

QString Backend::GetProjectsUrl() {
    return BaseUrl + "/project/all";
}

QString Backend::CreateProjectUrl() {
    return BaseUrl + "/project/create";
}

QString Backend::EditProjectUrl() {
    return BaseUrl + "/project/edit";
}

QString Backend::SignInAccountUrl() {
    return BaseUrl + "/user/enter";
}

QString Backend::SignUpAccountUrl() {
    return BaseUrl + "/user/create";
}

QString Backend::GetAccountUrl() {
    return BaseUrl + "/user/getByAccessToken";
}

QString Backend::EditAccountUrl() {
    return BaseUrl + "/user/edit";
}

QString Backend::GetTasksUrl() {
    return BaseUrl + "/task/all";
}

QString Backend::CreateTaskUrl() {
    return BaseUrl + "/task/create";
}

QString Backend::EditTaskUrl() {
    return BaseUrl + "/task/edit";
}

QString Backend::DeleteTaskUrl() {
    return BaseUrl + "/task/delete";
}

QString Backend::DeleteUserUrl() {
    return BaseUrl + "/user/delete";
}

QJsonObject Backend::GetRootFromReply(QNetworkReply *reply, Status &status) {
    QNetworkReply::NetworkError error = reply->error();

    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = json.object();

    status.response = QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(error);
    status.response += " :: " + root["status"].toString() + " >> " + root["description"].toString();

    status.isSuccess = (error == QNetworkReply::NoError) && root["status"].toBool();

    return root;
}

void Backend::PostRequest(const QString &urlString, const QMap<QString, QString> &args) {
    QUrlQuery query;

    for (auto& entry : args.toStdMap()) {
        query.addQueryItem(entry.first, entry.second);
    }

    QUrl url = QUrl(urlString);
    url.setQuery(query.toString(QUrl::FullyEncoded).toUtf8());
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    myNetworkManager->post(request, "");
    qInfo() << request.url();
}

void Backend::GetRequest(const QString &urlString, const QMap<QString, QString> &args) {
    QString fullUrl = urlString + "?";
    for (auto& entry : args.toStdMap()) {
        fullUrl += entry.first + "=" + entry.second + "&";
    }
    fullUrl.chop(1);
    QUrl url = QUrl(fullUrl);
    myNetworkManager->get(QNetworkRequest(url));
    qInfo() << url;
}

void Backend::SignIn(const QString& username, const QString& password) {
    GetRequest(SignInAccountUrl(), QMap<QString, QString> {
                   { "screen_name", username },
                   { "password", password }
               });
}

void Backend::CheckPassword(const QString& username, const QString& password) {
    GetRequest(SignInAccountUrl(), QMap<QString, QString> {
                   { "screen_name", username },
                   { "password", password },
                   { "to_check", "true"}
               });
}

void Backend::SignUp(const QString& fullName, const QString& username, const QString& email, const QString& password) {
    PostRequest(SignUpAccountUrl(), QMap<QString, QString> {
                    { "full_name", fullName },
                    { "screen_name", username },
                    { "email", email },
                    { "password", password }
                });
}

void Backend::GetProjects() {
    GetRequest(GetProjectsUrl(), QMap<QString, QString> {
                  { "access_token", myToken }
               });
}

void Backend::CreateProject(const QString &projectName) {
    // TODO: photo
    PostRequest(CreateProjectUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "full_name", projectName }
                });
}

void Backend::EditProject(const ProjectInfo& projectInfo) {
    // TODO: photo
    PostRequest(EditProjectUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "project_id", QString("%1").arg(projectInfo.GetId()) },
                    { "full_name", projectInfo.GetTitle() }
                });
};

void Backend::GetTasks(const ProjectInfo &projectInfo)
{
    GetRequest(GetTasksUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "project_id", QString("%1").arg(projectInfo.GetId()) }
               });
}

void Backend::CreateTask(const TaskInfo &taskInfo)
{
    PostRequest(CreateTaskUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "project_id", QString("%1").arg(taskInfo.GetProjectId()) },
                    { "title", taskInfo.GetTitle() },
                    { "description", taskInfo.GetDescription() }
                });
}

void Backend::DeleteTask(const TaskInfo &taskInfo)
{
    GetRequest(DeleteTaskUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "task_id", QString("%1").arg(taskInfo.GetId()) }
               });
}

void Backend::EditTask(const TaskInfo &taskInfo)
{
    QMap<QString, QString> query {
        { "access_token", myToken },
        { "task_id", QString("%1").arg(taskInfo.GetId()) },
        { "title", taskInfo.GetTitle() },
        { "description", taskInfo.GetDescription() },
        { "story_points", QString("%1").arg(taskInfo.GetStoryPoints()) }
    };
    if (taskInfo.GetAssignee().has_value()) {
        query.insert("assigned_id", QString("%1").arg(taskInfo.GetAssignee().value().GetId()));
    }
    if (taskInfo.GetDeadline().has_value()) {
        query.insert("deadline", taskInfo.GetDeadline().value().toString("dd-MM-yyyy HH:mm:ss.zzz"));
    }

    PostRequest(EditTaskUrl(), query);
}

UserInfo Backend::GetProfile()
{
    GetRequest(GetAccountUrl(), QMap<QString, QString> {
                       { "access_token", myToken }
                   });
    return myUserInfo;
}

void Backend::UpdateProfile(const UserInfo& user)
{
//    GetRequest(GetAccountUrl(), QMap<QString, QString> {
//                   { "access_token", myToken }
//               });
    QMap<QString, QString> query {
        { "access_token", myToken },
        { "full_name", QString("%1").arg(user.GetFullName()) },
        { "email", user.GetEmail() }
    };

    PostRequest(EditAccountUrl(), query);
}

void Backend::DeleteUser(const UserInfo& user)
{
    GetRequest(DeleteUserUrl(), QMap<QString, QString> {
                   { "access_token", myToken }
               });
}

void Backend::ResetPassword(const QString& new_password)
{
    QMap<QString, QString> query {
        { "access_token", myToken },
        { "password", new_password }
    };

    PostRequest(EditAccountUrl(), query);
}

void Backend::OnResponse(QNetworkReply* reply)
{
    Status status;
    QJsonObject root = GetRootFromReply(reply, status);


    qInfo() << status.isSuccess << " " << status.response;

    QString request = reply->request().url().toString();
    if (request.indexOf('?') == -1) {
        return;
    }

    QString pattern = request.sliced(0, request.indexOf('?'));

    if (pattern == GetProjectsUrl()) {
        QList<ProjectInfo> projects;

        if (status.isSuccess) {
            for (QJsonValueRef it : root["projects"].toArray()) {
                ProjectInfo project = ProjectInfo::ParseFromJson(it.toObject()["project"].toObject());
                projects.push_back(project);

                // TODO: process role description
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
            myToken = root["accessToken"].toString();
            myUserInfo = UserInfo::ParseFromJson(root["user"].toObject());
        }
        if (request.toStdString().find("to_check") == std::string::npos) {
            emit SignedIn(status);
        }
        else
        {
            emit Checked(status);
        }
    } else if (pattern == SignUpAccountUrl()) {
        if (status.isSuccess) {
            myToken = root["accessToken"].toString();
            myUserInfo = UserInfo::ParseFromJson(root["user"].toObject());
        }

        emit SignedUp(status);
    } else if (pattern == GetAccountUrl()) {
        if (status.isSuccess) {
            myUserInfo = UserInfo::ParseFromJson(root["user"].toObject());
        }

        emit ProfileUpdated(status, myUserInfo);
    } else if (pattern == GetTasksUrl()) {
        QList<TaskInfo> tasks;
        // TODO: process all information
        for (QJsonValueRef it : root["tasks"].toArray()) {
            TaskInfo task = TaskInfo::ParseFromJson(it.toObject());
            tasks.push_back(task);
        }

        emit TasksLoaded(status, tasks);
    } else if (pattern == CreateTaskUrl()) {
        emit TaskEdited(status, TaskInfo::ParseFromJson(root["task"].toObject()));
    } else if (pattern == EditTaskUrl()) {
        emit TaskEdited(status, TaskInfo::ParseFromJson(root["task"].toObject()));
    } else if (pattern == DeleteTaskUrl()) {
        emit TaskDeleted(status);
    } else if (pattern == DeleteUserUrl()){
    }

}
