#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QtCore>
#include <QtHttpServer>

#include "Project.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineOption host_option("db-host", "database host", "db-host");
    QCommandLineOption db_option  ("db-name", "database name", "db-name");
    QCommandLineOption user_option("db-user", "database user", "db-user");
    QCommandLineOption pswd_option("db-pswd", "database password", "db-pswd");

    QCommandLineOption server_host_option("server-host", "server host", "server-host", "0.0.0.0");
    QCommandLineOption server_port_option("server-port", "server port", "server-port", "8080");

    QCommandLineParser parser;
    parser.addOptions({host_option, db_option, user_option, pswd_option, server_host_option, server_port_option});
    parser.addHelpOption();
    parser.process(a.arguments());

    QString db_host = parser.value("db-host");
    QString db_name = parser.value("db-name");
    QString db_user = parser.value("db-user");
    QString db_pswd = parser.value("db-pswd");

    if (db_host.isEmpty() || db_name.isEmpty() || db_user.isEmpty() || db_pswd.isEmpty()) {
        qInfo() << "invalid command line arguments";
        return 0;
    }

    QString server_host = parser.value("server-host");
    QString server_port_str = parser.value("server-port");

    if (server_host.isEmpty() || server_port_str.isEmpty()) {
        qInfo() << "invalid host or port arguments";
        return 0;
    }

    bool server_port_ok;
    quint16 server_port = server_port_str.toInt(&server_port_ok);
    if (!server_port_ok) {
        qInfo() << "invalid value of port passing";
        return 0;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(db_host);
    db.setDatabaseName(db_name);
    db.setUserName(db_user);
    db.setPassword(db_pswd);

    if (!db.open()) {
        qInfo() << "failed connection to database";
        return 0;
    }

    Project::database = &db;

    QHttpServer http_server;
    http_server.route("/project/create/", &Project::Create);
    http_server.route("/project/delete/", &Project::Delete);
    http_server.route("/project/edit/", QHttpServerRequest::Method::POST, &Project::Edit);
    http_server.route("/project/get/", &Project::Get);

    const quint16 port = http_server.listen(QHostAddress(server_host), server_port);
    if (port == 0) {
        qInfo() << "failed listen port";
        return 0;
    }

    qInfo() << port;

    return a.exec();
}
