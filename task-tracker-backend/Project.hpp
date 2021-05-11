#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QSqlResult>
#include <QtCore>
#include <QVector>
#include <QtHttpServer>

#include "Account.hpp"

class Project {
public:
    static QSqlDatabase *database;

#define ARG(var_name, arg_name) \
    if (!request.query().hasQueryItem(arg_name))\
        return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, QString("missing %1").arg(arg_name)} };\
    QString var_name = request.query().queryItemValue(arg_name);

    struct ProjectModel {
        bool is_valid = false;

        int id;
        QString full_name;
        QString photo;
    };

    static ProjectModel GetProject(int project_id) {
        ProjectModel result;

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(GET_PROJECT_SQL)) {
            database->rollback();
            return result;
        }
        q.addBindValue(project_id);
        if (!q.exec()) {
            database->rollback();
            return result;
        }

        if (q.next()) {
            result.is_valid = true;
            result.id = q.value("id").toInt();
            result.full_name = q.value("full_name").toString();
            if (q.value("photo").isNull()){
                result.photo = "null";
            } else {
                result.photo = q.value("photo").toString();
            }
        }

        database->commit();

        return result;
    }

    struct UserProjectLink {
        bool is_valid = false;

        int id;
        int project_id;
        int user_id;
        int role_id;
    };

    static QVector<UserProjectLink> GetUserProjects(int user_id) {
        QVector<UserProjectLink> result;

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(GET_USER_PROJECTS_SQL)) {
            database->rollback();
            return result;
        }
        q.addBindValue(user_id);
        if (!q.exec()) {
            database->rollback();
            return result;
        }

        while (q.next()) {
            UserProjectLink link;

            link.is_valid = true;
            link.id = q.value("int").toInt();
            link.project_id = q.value("project_id").toInt();
            link.user_id = q.value("user_id").toInt();
            link.role_id = q.value("role_id").toInt();

            result.append(link);
        }

        database->commit();

        return result;
    }


    static QJsonObject Create(const QHttpServerRequest &request) {
        QJsonObject result;

        const char *FAIL_DESCRIPTION = "description";

        ARG(full_name, "full_name");
        ARG(access_token, "access_token");

        auto user = Account::GetUser(access_token);
        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(CREATE_PROJECT_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(full_name);
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        int project_id = q.lastInsertId().toInt();

        if (!q.prepare(ADD_USER_TO_PROJECT_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(project_id);
        q.addBindValue(user.id);
        q.addBindValue(0);
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Delete(const QHttpServerRequest &request) {
        QJsonObject result;

        const char *FAIL_DESCRIPTION = "description";

        ARG(raw_project_id, "project_id");
        ARG(access_token, "access_token");

        int project_id = raw_project_id.toInt();

        // todo in one query
        auto user = Account::GetUser(access_token);

        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        auto project = GetProject(project_id);

        if (!project.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid project_id"} };
        }

        bool ok = false;
        auto links = GetUserProjects(user.id);
        for (const auto& link : links) {
            if (link.project_id == project_id && link.role_id == 0) {
                ok = true;
                break;
            }
        }

        if (!ok) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid project_id"} };
        }
        // ~~~

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(DELETE_PROJECT_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(project.id);
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Edit(const QHttpServerRequest &request) {
        QJsonObject result;

        const char *FAIL_DESCRIPTION = "description";

        ARG(raw_project_id, "project_id");
        ARG(access_token, "access_token");
        ARG(raw_fields, "fields");

        int project_id = raw_project_id.toInt();
        QJsonDocument fields_doc = QJsonDocument::fromJson(raw_fields.toUtf8());
        if (fields_doc.isNull() || !fields_doc.isObject()) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid fields"} };
        }
        QJsonObject fields_obj = fields_doc.object();

        // todo in one query
        auto user = Account::GetUser(access_token);

        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        auto project = GetProject(project_id);

        if (!project.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid project_id"} };
        }

        bool ok = false;
        auto links = GetUserProjects(user.id);
        for (const auto& link : links) {
            if (link.project_id == project_id && link.role_id == 0) {
                ok = true;
                break;
            }
        }

        if (!ok) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid project_id"} };
        }
        // ~~~

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(UPDATE_PROJECT_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        if (fields_obj.contains("full_name")) {
            q.addBindValue(fields_obj["full_name"].toString());
        } else {
            q.addBindValue(project.full_name);
        }

        if (fields_obj.contains("photo")) {
            q.addBindValue(fields_obj["photo"].toString());
        } else if (project.photo != "null") {
            q.addBindValue(project.photo);
        } else {
            q.addBindValue( QVariant() );
        }

        q.addBindValue(project.id);

        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Get(const QHttpServerRequest &request) {
        QJsonArray result;

        const char *FAIL_DESCRIPTION = "description";

        ARG(access_token, "access_token");

        auto user = Account::GetUser(access_token);

        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        auto links = GetUserProjects(user.id);
        for (const auto& link : links) {
            QJsonObject dt;
            dt.insert("id", link.id);
            dt.insert("project_id", link.project_id);
            dt.insert("role_id", link.role_id);

            auto project = GetProject(link.project_id);
            dt.insert("full_name", project.full_name);
            dt.insert("photo", project.photo);

            result.append(dt);
        }

        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }
private:

    const static QString GET_PROJECT_SQL;
    const static QString ADD_USER_TO_PROJECT_SQL;
    const static QString GET_USER_PROJECTS_SQL;
    const static QString CREATE_PROJECT_SQL;
    const static QString UPDATE_PROJECT_SQL;
    const static QString DELETE_PROJECT_SQL;
};

QSqlDatabase *Project::database = nullptr;

const QString Project::GET_PROJECT_SQL = QString(R"(
    SELECT * FROM projects WHERE id = ?;
)");

const QString Project::ADD_USER_TO_PROJECT_SQL = QString(R"(
    INSERT INTO projectsusersroles(project_id, user_id, role_id) VALUES (?, ?, ?);
)");

const QString Project::GET_USER_PROJECTS_SQL = QString(R"(
    SELECT * FROM projectsusersroles WHERE user_id = ?;
)");

const QString Project::CREATE_PROJECT_SQL = QString(R"(
    INSERT INTO projects(full_name) VALUES (?);
)");

const QString Project::UPDATE_PROJECT_SQL = QString(R"(
    UPDATE projects SET full_name = ?, photo = ? WHERE id = ?;
)");

const QString Project::DELETE_PROJECT_SQL = QString(R"(
    DELETE FROM projects WHERE id = ?;
)");
