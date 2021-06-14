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
#include "LoadingBar.h"

Status::Status() : Status(true, "") {}

Status::Status(bool success, const QString &message) {
    isSuccess = success;
    response = message;
}

Backend Backend::Instance = Backend();

const QString Backend::BaseUrl = "http://139.59.144.121:8080";

Backend::Backend() : myNetworkManager(std::make_unique<QNetworkAccessManager>()) {
    connect(myNetworkManager.get(), &QNetworkAccessManager::finished, this, &Backend::OnResponse);
    //connect(this, Backend::SignInFailed, this, &LoadingBar::FailLoading);
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

QString Backend::GetProjectUsersUrl() {
    return BaseUrl + "/project/allUsers";
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

QString Backend::GetRolesUrl() {
    return BaseUrl + "/role/all";
}

QString Backend::CreateRoleUrl() {
    return BaseUrl + "/role/create";
}

QString Backend::EditRoleUrl() {
    return BaseUrl + "/role/edit";
}

QString Backend::DeleteRoleUrl() {
    return BaseUrl + "/role/delete";
}

QString Backend::InviteByEmailUrl() {
    return BaseUrl + "/projectUserRole/createByEmail";
}

QString Backend::KickUrl() {
    return BaseUrl + "/projectUserRole/deleteById";
}

QString Backend::ChangeRoleUrl() {
    return BaseUrl + "/projectUserRole/edit";
}

QString Backend::CreateCommentUrl() {
    return BaseUrl + "/comment/create";
}

QString Backend::DeleteCommentUrl() {
    return BaseUrl + "/comment/delete";
}

QString Backend::EditCommentUrl() {
    return BaseUrl + "/comment/edit";
}

QString Backend::GetCommentsUrl() {
    // TODO: change to actual
    return BaseUrl + "/comment/all";
}

QString Backend::CreateTagCaptionUrl() {
    return BaseUrl + "/tagCaption/create";
}

QString Backend::DeleteTagCaptionUrl() {
    return BaseUrl + "/tagCaption/delete";
}

QString Backend::EditTagCaptionUrl() {
    return BaseUrl + "/tagCaption/edit";
}

QString Backend::GetTagCaptionUrl() {
    return BaseUrl + "/tagCaption/all";
}

QString Backend::CreateTagValueUrl() {
    return BaseUrl + "/tagValue/create";
}

QString Backend::DeleteTagValueUrl() {
    return BaseUrl + "/tagValue/delete";
}

QString Backend::EditTagValueUrl() {
    return BaseUrl + "/tagValue/edit";
}

QString Backend::AddTagUrl() {
    return BaseUrl + "/taskTag/create";
}

QString Backend::RemoveTagUrl() {
    return BaseUrl + "/taskTag/delete";
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

    myRequestCounting++;
    emit LoadingChanged(true);
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

    myRequestCounting++;
    emit LoadingChanged(true);
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
}

void Backend::GetProjectUsers(const ProjectInfo &projectInfo) {
    GetRequest(GetProjectUsersUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "project_id", QString("%1").arg(projectInfo.GetId()) }
               });
};

void Backend::GetTasks(const ProjectInfo &projectInfo) {
    GetRequest(GetTasksUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "project_id", QString("%1").arg(projectInfo.GetId()) }
               });
}

void Backend::CreateTask(const TaskInfo &taskInfo) {
    PostRequest(CreateTaskUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "project_id", QString("%1").arg(taskInfo.GetProjectId()) },
                    { "title", taskInfo.GetTitle() },
                    { "description", taskInfo.GetDescription() }
                });
}

void Backend::DeleteTask(const TaskInfo &taskInfo) {
    GetRequest(DeleteTaskUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "task_id", QString("%1").arg(taskInfo.GetId()) }
               });
}

void Backend::EditTask(const TaskInfo &taskInfo) {
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

void Backend::UpdateUser(const UserInfo& user)
{
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

void Backend::UpdateProfile() {
    GetRequest(GetAccountUrl(), QMap<QString, QString> {

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

void Backend::GetRoles(const ProjectInfo &projectInfo) {
    GetRequest(GetRolesUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "project_id", QString("%1").arg(projectInfo.GetId()) }
               });
}

void Backend::CreateRole(const RoleInfo &roleInfo) {
    PostRequest(CreateRoleUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "project_id", QString("%1").arg(roleInfo.GetProjectId()) },
                    { "value", roleInfo.GetCaption() },
                    { "permissions", roleInfo.GetPermissionStr() }
                });
}

void Backend::EditRole(const RoleInfo &roleInfo) {
    PostRequest(EditRoleUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "role_id", QString("%1").arg(roleInfo.GetId()) },
                    { "value", roleInfo.GetCaption() },
                    { "permissions", roleInfo.GetPermissionStr() }
                });
}

void Backend::DeleteRole(const RoleInfo &roleInfo) {
    GetRequest(DeleteRoleUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "role_id", QString("%1").arg(roleInfo.GetId()) }
               });
}

void Backend::InviteByEmail(const ProjectInfo &project, const RoleInfo &role, const QString &email) {
    PostRequest(InviteByEmailUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "email", email },
                    { "project_id", QString("%1").arg(project.GetId()) },
                    { "role_id", QString("%1").arg(role.GetId()) }
                });
}

void Backend::Kick(const ProjectInfo &project, const UserInfo &user) {
    GetRequest(KickUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "user_id", QString("%1").arg(user.GetId()) },
                   { "project_id", QString("%1").arg(project.GetId()) }
               });
}

void Backend::ChangeRole(const UserInfo &user, const RoleInfo &role) {
    PostRequest(ChangeRoleUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "user_id", QString("%1").arg(user.GetId()) },
                    { "role_id", QString("%1").arg(role.GetId()) }
                });

}

void Backend::CreateComment(const TaskInfo& task, const CommentInfo &comment) {
    PostRequest(CreateCommentUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "task_id", QString("%1").arg(task.GetId()) },
                    { "text", comment.GetText() }
                });
}

void Backend::DeleteComment(const CommentInfo &comment) {
    GetRequest(DeleteCommentUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "comment_id", QString("%1").arg(comment.GetId()) }
               });
}

void Backend::EditComment(const CommentInfo &comment) {
    PostRequest(EditCommentUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "comment_id", QString("%1").arg(comment.GetId()) },
                    { "text", comment.GetText() }
                });
}

void Backend::GetComments(const TaskInfo &task) {
    GetRequest(GetCommentsUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "task_id", QString("%1").arg(task.GetId())}
               });
}

void Backend::CreateTagCaption(const ProjectInfo &project, const QString &tagCaption) {
    PostRequest(CreateTagCaptionUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "project_id", QString("%1").arg(project.GetId()) },
                    { "caption", tagCaption }
                });
}

void Backend::DeleteTagCaption(const TagInfo &tag) {
    GetRequest(DeleteTagCaptionUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "tag_caption_id", QString("%1").arg(tag.GetId()) }
               });
}

void Backend::EditTagCaption(const TagInfo &tag) {
    PostRequest(EditTagCaptionUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "tag_caption_id", QString("%1").arg(tag.GetId()) },
                    { "caption", tag.GetCaption() }
                });
}

void Backend::GetTagCaptions(const ProjectInfo &project) {
    GetRequest(GetTagCaptionUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "project_id", QString("%1").arg(project.GetId()) }
               });
}

void Backend::CreateTagValue(const TagInfo &tag, const QString &tagValue) {
    PostRequest(CreateTagValueUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "tag_caption_id", QString("%1").arg(tag.GetId()) },
                    { "value", tagValue }
                });
}

void Backend::DeleteTagValue(const TagValue &tagValue) {
    GetRequest(DeleteTagValueUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "tag_value_id", QString("%1").arg(tagValue.GetId()) }
               });
}

void Backend::EditTagValue(const TagValue &tagValue) {
    PostRequest(EditTagValueUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "tag_value_id", QString("%1").arg(tagValue.GetId()) },
                    { "value", tagValue.GetValue() }
                });
}

void Backend::AddTag(const TaskInfo &task, const TagValue &tag) {
    PostRequest(AddTagUrl(), QMap<QString, QString> {
                    { "access_token", myToken },
                    { "task_id", QString("%1").arg(task.GetId()) },
                    { "tag_value_id", QString("%1").arg(tag.GetId()) }
                });
}

void Backend::RemoveTag(const TaskTag &taskTag) {
    GetRequest(RemoveTagUrl(), QMap<QString, QString> {
                   { "access_token", myToken },
                   { "task_tag_id", QString("%1").arg(taskTag.GetId()) }
               });

}

void Backend::OnResponse(QNetworkReply* reply) {
    Status status;
    myRequestCounting--;
    QJsonObject root = GetRootFromReply(reply, status);

    qInfo() << status.isSuccess << " " << status.response;

    QString request = reply->request().url().toString();
    if (request.indexOf('?') == -1) {
        return;
    }

    QString pattern = request.sliced(0, request.indexOf('?'));

    if (pattern == GetProjectsUrl()) {
        QList<QPair<ProjectInfo, RoleInfo>> projects;

        if (status.isSuccess) {
            for (QJsonValueRef it : root["projects"].toArray()) {
                ProjectInfo project = ProjectInfo::ParseFromJson(it.toObject()["project"].toObject());
                RoleInfo role = RoleInfo::ParseFromJson(it.toObject()["role"].toObject());
                projects.push_back(QPair<ProjectInfo, RoleInfo>(project, role));
            }
        } else {
            qInfo() << "ошибочка вышла..";
            emit RequestFailed("Не удалось получить данные о проекте.");
        }

        emit ProjectsLoaded(status, projects);
    } else if (pattern == CreateProjectUrl()) {
        GetProjects();

        emit ProjectCreated(status);
    } else if (pattern == EditProjectUrl()) {
        if (status.isSuccess) {
            GetProjects();
        } else {
            qInfo() << "ошибочка вышла..";
            emit RequestFailed("Не удалось сохранить изменения.");
        }

        emit ProjectEdited(status);
    } else if (pattern == GetProjectUsersUrl()) {
        QList<QPair<UserInfo, RoleInfo>> users;
        if (status.isSuccess) {
            for (QJsonValueRef it : root["users"].toArray()) {
                QJsonObject obj = it.toObject();
                UserInfo user = UserInfo::ParseFromJson(obj["user"].toObject());
                RoleInfo role = RoleInfo::ParseFromJson(obj["role"].toObject());
                users.push_back(QPair<UserInfo, RoleInfo>(user, role));
            }
        }

        emit ProjectUsersLoaded(status, users);
    } else if (pattern == SignInAccountUrl()) {
        UserInfo user = Context::DEFAULT_USER;
        if (status.isSuccess) {
            myToken = root["accessToken"].toString();
            user = UserInfo::ParseFromJson(root["user"].toObject());
        } else {
            qInfo() << "ошибочка вышла..";
            emit RequestFailed("Неправильный логин или пароль!");
        }
        if (request.toStdString().find("to_check") == std::string::npos) {
            emit SignedIn(status, user);
        }
        else
        {
            emit Checked(status);
        }

        //emit SignedIn(status, user);
    } else if (pattern == SignUpAccountUrl()) {
        UserInfo user = Context::DEFAULT_USER;
        if (status.isSuccess) {
            myToken = root["accessToken"].toString();
            user = UserInfo::ParseFromJson(root["user"].toObject());
        } else {
            qInfo() << "ошибочка вышла..";
            emit RequestFailed("Вы не заполнили все обязательные поля!");
        }

        emit SignedUp(status, user);
    } else if (pattern == GetAccountUrl()) {
        UserInfo user = Context::DEFAULT_USER;
        if (status.isSuccess) {
            user = UserInfo::ParseFromJson(root["user"].toObject());
        } else {
            qInfo() << "ошибочка вышла..";
            emit RequestFailed("Не удалось получить url аккаунта.");
        }

        emit ProfileUpdated(status, user);

    } else if (pattern == EditAccountUrl()){
        UserInfo user = Context::DEFAULT_USER;
        if (status.isSuccess) {
            user = UserInfo::ParseFromJson(root["user"].toObject());

            emit ProfileUpdated(status, user);
        }


    } else if (pattern == GetTasksUrl()) {
        QList<TaskInfo> tasks;
        // TODO: process all information
        if (status.isSuccess) {
            for (QJsonValueRef it : root["tasks"].toArray()) {
                TaskInfo task = TaskInfo::ParseFromJson(it.toObject());
                tasks.push_back(task);
            }
        }

        emit TasksLoaded(status, tasks);
    } else if (pattern == CreateTaskUrl()) {
        emit TaskEdited(status, TaskInfo::ParseFromJson(root["task"].toObject()));
    } else if (pattern == EditTaskUrl()) {
        emit TaskEdited(status, TaskInfo::ParseFromJson(root["task"].toObject()));
    } else if (pattern == DeleteTaskUrl()) {
        emit TaskDeleted(status);
    } else if (pattern == DeleteUserUrl()){

    } else if (pattern == GetRolesUrl()) {
        QList<RoleInfo> roles;
        if (status.isSuccess) {
            for (QJsonValueRef it : root["roles"].toArray()) {
                roles.push_back(RoleInfo::ParseFromJson(it.toObject()));
            }
        }

        emit RolesLoaded(status, roles);
    } else if (pattern == CreateRoleUrl()) {
        RoleInfo role(-1, "", 0, -1);
        if (status.isSuccess) {
            role = RoleInfo::ParseFromJson(root["role"].toObject());
        }

        emit RoleCreated(status, role);
    } else if (pattern == EditRoleUrl()) {
        RoleInfo role(-1, "", 0, -1);
        if (status.isSuccess) {
            role = RoleInfo::ParseFromJson(root["role"].toObject());
        }

        emit RoleEdited(status, role);
    } else if (pattern == DeleteRoleUrl()) {
        emit RoleDeleted(status);
    } else if (pattern == InviteByEmailUrl()) {
        emit MemberInvited(status);
    } else if (pattern == KickUrl()) {
        emit MemberKicked(status);
    } else if (pattern == ChangeRoleUrl()) {
        emit RoleChanged(status);
    } else if (pattern == CreateCommentUrl()) {
        CommentInfo comment(-1, std::optional<UserInfo>(), QDateTime(), "");
        if (status.isSuccess) {
            comment = CommentInfo::ParseFromJson(root["comment"].toObject());
        }

        emit CommentCreated(status, comment);
    } else if (pattern == DeleteCommentUrl()) {
        emit CommentDeleted(status);
    } else if (pattern == EditCommentUrl()) {
        CommentInfo comment(-1, std::optional<UserInfo>(), QDateTime(), "");
        if (status.isSuccess) {
            comment = CommentInfo::ParseFromJson(root["comment"].toObject());
        }

        emit CommentEdited(status, comment);
    } else if (pattern == GetCommentsUrl()) {
        QList<CommentInfo> comments;

        if (status.isSuccess) {
            for (QJsonValueRef obj : root["comments"].toArray()) {
                comments.push_back(CommentInfo::ParseFromJson(obj.toObject()));
            }
        }

        emit CommentsLoaded(status, comments);
    } else if (pattern == CreateTagCaptionUrl()) {
        TagInfo tag(-1, -1, "", QList<TagValue>());
        if (status.isSuccess) {
            tag = TagInfo::ParseFromJson(root["tagCaption"].toObject());
        }
        emit TagCaptionCreated(status, tag);
    } else if (pattern == DeleteTagCaptionUrl()) {
        emit TagCaptionDeleted(status);
    } else if (pattern == EditTagCaptionUrl()) {
        TagInfo tag(-1, -1, "", QList<TagValue>());
        if (status.isSuccess) {
            tag = TagInfo::ParseFromJson(root["tagCaption"].toObject());
        }
        emit TagCaptionEdited(status, tag);
    } else if (pattern == GetTagCaptionUrl()) {
        QList<TagInfo> tags;
        if (status.isSuccess) {
            for (QJsonValueRef obj : root["tagCaptions"].toArray()) {
                tags.push_back(TagInfo::ParseFromJson(obj.toObject()));
            }
        }
        emit TagCaptionsLoaded(status, tags);
    } else if (pattern == CreateTagValueUrl()) {
        TagInfo tag(-1, -1, "", QList<TagValue>());
        TagValue tagValue(-1, "");
        if (status.isSuccess) {
            tag = TagInfo::ParseFromJson(root["tagCaption"].toObject());
            tagValue = TagValue::ParseFromJson(root["tagValue"].toObject());
        }

        emit TagCaptionEdited(status, tag);
    } else if (pattern == DeleteTagValueUrl()) {
        TagInfo tag(-1, -1, "", QList<TagValue>());
        if (status.isSuccess) {
            tag = TagInfo::ParseFromJson(root["tagCaption"].toObject());
        }

        emit TagCaptionEdited(status, tag);
    } else if (pattern == EditTagCaptionUrl()) {
        TagInfo tag(-1, -1, "", QList<TagValue>());
        TagValue tagValue(-1, "");
        if (status.isSuccess) {
            tag = TagInfo::ParseFromJson(root["tagCaption"].toObject());
            tagValue = TagValue::ParseFromJson(root["tagValue"].toObject());
        }

        emit TagCaptionEdited(status, tag);
    } else if (pattern == AddTagUrl()) {
        TaskTag taskTag(-1, TagValue(-1, ""));
        if (status.isSuccess) {
            taskTag = TaskTag::ParseFromJson(root["taskTag"].toObject());
        }
        emit TagAdded(status, taskTag);
    } else if (pattern == RemoveTagUrl()) {
        emit TagRemoved(status);
    }

    if (myRequestCounting == 0) {
        emit LoadingChanged(false);
    }

}
