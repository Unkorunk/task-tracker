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

class Task {
public:
    static QSqlDatabase *database;

    static QJsonObject Create(const QHttpServerRequest &request) {
        QJsonObject result;
        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Delete(const QHttpServerRequest &request) {
        QJsonObject result;
        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Get(const QHttpServerRequest &request) {
        QJsonObject result;
        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

    static QJsonObject Edit(const QHttpServerRequest &request) {
        QJsonObject result;
        return QJsonObject{
            { "status", "ok" },
            { "data", result }
        };
    }

private:

};

QSqlDatabase *Task::database = nullptr;
