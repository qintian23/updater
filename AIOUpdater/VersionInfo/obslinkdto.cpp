#include "obslinkdto.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

obsLinkDto::obsLinkDto()
    : code(0)
    , success(false)
    , message(" ")
    , extras(" ")
    , data(" ")
    , timestamp(" ")
    , errorcode(" ")
{

}

bool obsLinkDto::init(QByteArray responce)
{
    bool ret = false;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(responce, &jsonError);
    if(!document.isNull() && (QJsonParseError::NoError == jsonError.error))
    {
       QJsonObject val = document.object();
       this->set_code(val["code"].toInt());
       this->set_data(val["data"].toString());
       this->set_extras(val["extras"].toString());
       this->set_message(val["message"].toString());
       this->set_success(val["success"].toBool());
       this->set_errorcode(val["errorcode"].toString());
       this->set_timestamp(val["timestamp"].toString());
       ret = true;
    }
    return ret;
}

QByteArray obsLinkDto::toString()
{
    QJsonObject jsonParam;
    jsonParam.insert("code", code);
    jsonParam.insert("success",success);
    jsonParam.insert("message", message);
    jsonParam.insert("extras(", extras);
    jsonParam.insert("data", data);
    jsonParam.insert("timestamp", timestamp);
    jsonParam.insert("errorcode", errorcode);
    QJsonDocument doc;
    doc.setObject(jsonParam);
    QByteArray param = doc.toJson(QJsonDocument::Compact);
    return param;
}
