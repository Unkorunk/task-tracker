#ifndef RESPONSEHELPER_HPP
#define RESPONSEHELPER_HPP

#include <type_traits>

#include <QJsonObject>
#include <QJsonDocument>
#include <httplib.h>

class ResponseHelper {
public:
    enum class Status {
        OK, FAIL
    };

    static void Send(httplib::Response &res, Status status, const QJsonObject& json_object) {
        using T = std::underlying_type<Status>::type;

        QJsonDocument json_document;
        QJsonObject obj{
            {"status", static_cast<T>(status)}
        };

        obj.insert("data", json_object);

        json_document.setObject(obj);

        res.set_content(json_document.toJson(QJsonDocument::Compact).toStdString(), "application/json");
    }
};


#endif // RESPONSEHELPER_HPP
