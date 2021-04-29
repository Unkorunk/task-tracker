#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <QtSql/QSqlDatabase>
#include <httplib.h>
#include "ResponseHelper.hpp"

class Account {
public:
    static QSqlDatabase *database;

    static void SignIn(const httplib::Request &req, httplib::Response &res) {
        QJsonObject result;

        result.insert("access_token", "abra");

        ResponseHelper::Send(res, ResponseHelper::Status::OK, result);
    }

    static void SignUp(const httplib::Request &req, httplib::Response &res) {
        QJsonObject result;

        result.insert("message", "fwfwq");

        ResponseHelper::Send(res, ResponseHelper::Status::FAIL, result);
    }

};

QSqlDatabase *Account::database = nullptr;

#endif // ACCOUNT_HPP
