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

class Task {
public:
#define ARG(var_name, arg_name) \
    if (!request.query().hasQueryItem(arg_name))\
        return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, QString("missing %1").arg(arg_name)} };\
    QString var_name = request.query().queryItemValue(arg_name);

    static QSqlDatabase *database;

    static QJsonObject Create(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(access_token, "access_token");
        ARG(project_id, "project_id");
        ARG(story_points, "story_points");
        ARG(title, "title");
        ARG(description, "description");

        auto user = Account::GetUser(access_token);
        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(CREATE_TASK_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(user.id);
        q.addBindValue(user.id);
        q.addBindValue(project_id);
        q.addBindValue(story_points);
        q.addBindValue(title);
        q.addBindValue(description);
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" }
        };
    }

    static QJsonObject Delete(const QHttpServerRequest &request) {

        const char *FAIL_DESCRIPTION = "description";

        ARG(id, "id");

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(DELETE_TASK_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(id);
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" }
        };
    }

    static QJsonObject Get(const QHttpServerRequest &request) {

        const char *FAIL_DESCRIPTION = "description";

        ARG(project_id, "project_id");

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(GET_TASK_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        q.addBindValue(project_id);

        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        QMap<int, QJsonObject> raw_result;

        while (q.next()) {
            const int task_id = q.value( "id" ).toInt();

            if (!raw_result.contains(task_id)) {
                QJsonObject task_result;

                task_result.insert( "id", task_id );
                task_result.insert( "created_by", q.value( "created_by" ).toInt() );
                task_result.insert( "created_at", q.value( "created_at" ).toString() );
                task_result.insert( "updated_by", q.value( "updated_by" ).toInt() );
                task_result.insert( "updated_at", q.value( "updated_at" ).toString() );
                task_result.insert( "project_id", q.value( "project_id" ).toInt() );
                task_result.insert( "story_points", q.value( "story_points" ).toInt() );
                task_result.insert( "title", q.value( "title" ).toString() );
                task_result.insert( "description", q.value( "description" ).toString() );
                QJsonArray property_value_ids;
                property_value_ids.append( q.value( "tag_value_id" ).toInt() );
                task_result.insert( "property_value_ids", property_value_ids );

                raw_result.insert( task_id, task_result );
            } else {
                auto property_value_ids = raw_result[task_id]["property_value_ids"].toArray();
                property_value_ids.append( q.value( "tag_value_id" ).toInt() );
                raw_result[task_id]["property_value_ids"] = property_value_ids;
            }
        }

        QJsonArray result;
        for ( auto it : raw_result ) {
            result.append( it );
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Edit(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(access_token, "access_token");
        ARG(id, "id");

        auto user = Account::GetUser(access_token);

        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        QString st = " UPDATE tasks SET updated_by = ?, updated_at = ?";
        if (request.query().hasQueryItem("story_points")) {
            st += ", story_points = ?";
        }
        if (request.query().hasQueryItem("title")) {
            st += ", title = ?";
        }
        if (request.query().hasQueryItem("description")) {
            st += ", description = ?";
        }
        st += " WHERE id = ?;";

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(st)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }


        q.addBindValue( user.id );
        q.addBindValue( QDateTime::currentDateTime() );
        if (request.query().hasQueryItem("story_points")) {
            q.addBindValue(request.query().queryItemValue("story_points").toInt());
        }
        if (request.query().hasQueryItem("title")) {
            q.addBindValue(request.query().queryItemValue("title"));
        }
        if (request.query().hasQueryItem("description")) {
            q.addBindValue(request.query().queryItemValue("description"));
        }
        q.addBindValue(id);

        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" }
        };
    }

    static QJsonObject GetProperties(const QHttpServerRequest &request) {

        const char *FAIL_DESCRIPTION = "description";


        database->transaction();

        QSqlQuery q;
        if (!q.prepare(GET_PROPERTIES_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        QMap<int, QJsonObject> raw_result;



        while (q.next()) {
            const int property_id = q.value( "property_id" ).toInt();

            if (!raw_result.contains(property_id)) {
                QJsonObject task_result;

                task_result.insert( "property_id", property_id );
                task_result.insert( "caption", q.value( "caption" ).toString() );

                QJsonArray property_values;

                QJsonObject value;
                value.insert( "id", q.value( "property_value_id" ).toInt() );
                value.insert( "title", q.value( "value" ).toString() );

                property_values.append( value );

                task_result.insert( "property_values", property_values );

                raw_result.insert( property_id, task_result );
            } else {
                auto property_values = raw_result[property_id]["property_values"].toArray();

                QJsonObject value;
                value.insert( "id", q.value( "property_value_id" ).toInt() );
                value.insert( "title", q.value( "value" ).toString() );

                property_values.append( value );

                raw_result[property_id]["property_values"] = property_values;
            }
        }

        QJsonArray result;
        for ( auto it : raw_result ) {
            result.append( it );
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject AddProperties(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(task_id, "task_id");
        ARG(proprty_value_id, "proprty_value_id");

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(ADD_PROPERTIES_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(task_id.toInt());
        q.addBindValue(proprty_value_id.toInt());
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" }
        };
    }

    static QJsonObject RemoveProperties(const QHttpServerRequest &request) {

        const char *FAIL_DESCRIPTION = "description";

        ARG(task_id, "task_id");
        ARG(proprty_value_id, "proprty_value_id");

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(REMOVE_PROPERTIES_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }
        q.addBindValue(task_id.toInt());
        q.addBindValue(proprty_value_id.toInt());
        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        return QJsonObject{
            { "status", "ok" }
        };
    }

private:
    const static QString CREATE_TASK_SQL;
    const static QString DELETE_TASK_SQL;
    const static QString GET_TASK_SQL;
    const static QString GET_PROPERTIES_SQL;
    const static QString ADD_PROPERTIES_SQL;
    const static QString REMOVE_PROPERTIES_SQL;
};

QSqlDatabase *Task::database = nullptr;

const QString Task::CREATE_TASK_SQL = R"(
    INSERT INTO tasks(created_by, updated_by, project_id, story_points, title, description) VALUES (?, ?, ?, ?, ?, ?);
)";

const QString Task::DELETE_TASK_SQL = R"(
    DELETE FROM tasks WHERE id = ?;
)";

const QString Task::GET_TASK_SQL = R"(
    SELECT * FROM tasks LEFT JOIN taskstags ON tasks.id = taskstags.task_id WHERE project_id = ?;
)";

const QString Task::GET_PROPERTIES_SQL = R"(
    SELECT tagcaptions.id as property_id, caption, tagvalues.id as property_value_id, value FROM tagcaptions LEFT JOIN tagvalues ON tagcaptions.id = tagvalues.tagcaption_id;
)";

const QString Task::ADD_PROPERTIES_SQL = R"(
    INSERT INTO taskstags(task_id, tag_value_id) VALUES (?, ?);
)";

const QString Task::REMOVE_PROPERTIES_SQL = R"(
    DELETE FROM taskstags WHERE task_id = ? AND tag_value_id = ?;
)";
