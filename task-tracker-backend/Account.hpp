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
#include <QCryptographicHash>
#include <QDateTime>

class Account {
public:
    static QSqlDatabase *database;


#define ARG(var_name, arg_name) \
    if (!request.query().hasQueryItem(arg_name))\
        return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, QString("missing %1").arg(arg_name)} };\
    QString var_name = request.query().queryItemValue(arg_name);


    struct UserModel {
        bool is_valid = false;

        int id;
        QString full_name;
        QString username;
        QString password;
        QString photo;
        QString email;
        QString access_token;
        int valid_until;
    };

    struct ResultWithDescription {
        bool result;
        QString description;
    };

    static ResultWithDescription CreateUser(const QString& full_name, const QString& username, const QString& email, const QString& password) {
        ResultWithDescription result;
        result.result = true;

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(CREATE_USER_SQL)) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }
        q.addBindValue(full_name);
        q.addBindValue(username);
        q.addBindValue(email);
        QString hash_pswd = QCryptographicHash::hash( password.toUtf8(), QCryptographicHash::Sha256 ).toHex();
        q.addBindValue(hash_pswd);
        if (!q.exec()) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }

        database->commit();

        return result;
    }

    static ResultWithDescription DeleteUser(const QString& access_token) {
        ResultWithDescription result;
        result.result = true;

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(DELETE_USER_SQL)) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }
        q.addBindValue(access_token);
        if (!q.exec()) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }

        database->commit();

        return result;
    }

    static UserModel GetUser(const QString& access_token) {
        UserModel result;

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(GET_USER_SQL)) {
            database->rollback();
            return result;
        }
        q.addBindValue(access_token);
        if (!q.exec()) {
            database->rollback();
            return result;
        }

        if (q.next()) {
            result.id = q.value("id").toInt();
            result.full_name = q.value("full_name").toString();
            result.username = q.value("username").toString();
            result.password = q.value("password").toString();
            if (q.value("photo").isNull()) {
                result.photo = "null";
            } else {
                result.photo = q.value("photo").toString();
            }
            result.email = q.value("email").toString();
            if (q.value("access_token").isNull()) {
                result.access_token = "null";
            } else {
                result.access_token = q.value("access_token").toString();
            }
            result.valid_until = q.value("valid_until").toInt();
            result.is_valid = ( result.valid_until > QDateTime::currentDateTime().toSecsSinceEpoch() );
        }

        database->commit();

        return result;
    }

    static ResultWithDescription EnterUser(const QString& username, const QString& password)
    {
        ResultWithDescription result;
        result.result = true;

        database->transaction();

        QSqlQuery q;
        if (!q.prepare(ENTER_USER_SQL)) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }
        q.addBindValue(username);
        QString hash_pswd = QCryptographicHash::hash( password.toUtf8(), QCryptographicHash::Sha256 ).toHex();
        q.addBindValue(hash_pswd);
        if (!q.exec()) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }

        if (!q.next())
        {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }

        const QString token_salt = "CfHWkwP4uBH94ec7pDgjGmXu";

        QString token = QCryptographicHash::hash( QString( "%1%2%3%4" )
                                               .arg( QDateTime::currentDateTime().toSecsSinceEpoch() )
                                               .arg( username   )
                                               .arg( token_salt )
                                               .arg( hash_pswd  )
                                               .toUtf8(), QCryptographicHash::Md5 ).toHex();

        if (!q.prepare(UPDATE_TOKEN_SQL)) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }
        q.addBindValue( token );
        q.addBindValue( QDateTime::currentDateTime().addDays(1).toSecsSinceEpoch() );
        q.addBindValue( username );

        if (!q.exec()) {
            database->rollback();
            result.description = q.lastError().text();
            result.result = false;
            return result;
        }

        database->commit();

        result.description = token;

        return result;
    }

    static QJsonObject Create(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(full_name, "full_name");
        ARG(username, "username");
        ARG(email, "email");
        ARG(password, "password");

        auto result = CreateUser(full_name, username, email, password);

        if (!result.result)
        {
            return QJsonObject{
                { "status", "fail" },
                { FAIL_DESCRIPTION, result.description }
            };
        }

        result = EnterUser(username, password);

        if (!result.result)
        {
            return QJsonObject{
                { "status", "fail" },
                { FAIL_DESCRIPTION, result.description }
            };
        }

        QJsonObject data;
        data.insert("access_token", result.description);

        return QJsonObject{
            { "status", "ok" },
            { "data", data }
        };
    }

    static QJsonObject Delete(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(access_token, "access_token");

        auto result = DeleteUser(access_token);

        if (!result.result)
        {
            return QJsonObject{
                { "status", "fail" },
                { FAIL_DESCRIPTION, result.description }
            };
        }

        return QJsonObject{
            { "status", "ok" }
        };
    }

    static QJsonObject Get(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(access_token, "access_token");

        auto result = GetUser(access_token);

        QJsonObject data;

        if (!result.is_valid)
        {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        data.insert("id", result.id);
        data.insert("full_name", result.full_name);
        data.insert("username", result.username);
        data.insert("photo", result.photo);
        data.insert("email", result.email);

        return QJsonObject{
            { "status", "ok" },
            { "data", data }
        };
    }

    static QJsonObject Enter(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(username, "username");
        ARG(password, "password");

        auto result = EnterUser(username, password);

        if (!result.result)
        {
            return QJsonObject{
                { "status", "fail" },
                { FAIL_DESCRIPTION, result.description }
            };
        }

        QJsonObject data;
        data.insert("access_token", result.description);

        return QJsonObject{
            { "status", "ok" },
            { "data", data }
        };
    }

    static QJsonObject Edit(const QHttpServerRequest &request) {
        const char *FAIL_DESCRIPTION = "description";

        ARG(access_token, "access_token");
        ARG(confirm_password, "confirm_password");
        ARG(raw_fields, "fields");

        auto user = GetUser(access_token);
        if (!user.is_valid) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid access_token"} };
        }

        QString hash_pswd = QCryptographicHash::hash( confirm_password.toUtf8(), QCryptographicHash::Sha256 ).toHex();

        QJsonDocument fields_doc = QJsonDocument::fromJson(raw_fields.toUtf8());
        if (fields_doc.isNull() || !fields_doc.isObject()) {
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, "invalid fields"} };
        }
        QJsonObject fields_obj = fields_doc.object();

        database->transaction();


        QSqlQuery q;
        if (!q.prepare(EDIT_USER_SQL)) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        if (fields_obj.contains("full_name")) {
            q.addBindValue(fields_obj["full_name"].toString());
        } else {
            q.addBindValue(user.full_name);
        }

        if (fields_obj.contains("password")) {
            QString hash_pswd = QCryptographicHash::hash( fields_obj["password"].toString().toUtf8(), QCryptographicHash::Sha256 ).toHex();
            q.addBindValue(hash_pswd);
        } else {
            q.addBindValue(user.password);
        }

        if (fields_obj.contains("photo")) {
            q.addBindValue(fields_obj["photo"].toString());
        } else if (user.photo != "null") {
            q.addBindValue(user.photo);
        } else {
            q.addBindValue( QVariant() );
        }

        if (fields_obj.contains("email")) {
            q.addBindValue(fields_obj["email"].toString());
        } else {
            q.addBindValue(user.email);
        }

        q.addBindValue(access_token);
        q.addBindValue(hash_pswd);

        if (!q.exec()) {
            database->rollback();
            return QJsonObject{ {"status", "fail"}, {FAIL_DESCRIPTION, q.lastError().text()} };
        }

        database->commit();

        QJsonObject result;
        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

private:
    const static QString CREATE_USER_SQL;
    const static QString DELETE_USER_SQL;
    const static QString GET_USER_SQL;
    const static QString ENTER_USER_SQL;
    const static QString UPDATE_TOKEN_SQL;
    const static QString EDIT_USER_SQL;
};

QSqlDatabase *Account::database = nullptr;

const QString Account::CREATE_USER_SQL= QString(R"(
    INSERT INTO users(full_name, username, email, password) VALUES (?, ?, ?, ?);
)");

const QString Account::DELETE_USER_SQL= QString(R"(
    DELETE FROM users WHERE access_token = ?;
)");

const QString Account::GET_USER_SQL = QString(R"(
    SELECT * FROM users WHERE access_token = ?;
)");

const QString Account::ENTER_USER_SQL= QString(R"(
    SELECT * FROM users WHERE username = ? AND password = ?;
)");

const QString Account::UPDATE_TOKEN_SQL = QString(R"(
    UPDATE users SET access_token = ?, valid_until = ? WHERE username = ?;
)");

const QString Account::EDIT_USER_SQL= QString(R"(
    UPDATE users SET full_name = ?, password = ?, photo = ?, email = ? WHERE access_token = ? AND password = ?;
)");
