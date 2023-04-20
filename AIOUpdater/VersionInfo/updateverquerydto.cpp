#include "updateverquerydto.h"
#include "upgradeaccess.h"
#include "httpcommon.h"

#include <QObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>

UpdateVerQueryDto::UpdateVerQueryDto()
    : id("")
    , clientType(0)
    , version("")
    , isEnabled(true)
    , isDeleted(true)
    , updateContent("")
    , updateFileId("")
    , remark("")
    , operators("")
    , operateTime("")
    , creator("")
    , createTime("")
{

}

bool UpdateVerQueryDto::init(QJsonObject val)
{
    bool ret = false;
    if(!val.isEmpty())
    {
        this->set_id(val["id"].toVariant().toString());
        this->set_remark(val["remark"].toVariant().toString());
        this->set_creator(val["creator"].toVariant().toString());
        this->set_version(val["version"].toVariant().toString());
        this->set_isDeleted(val["isDeleted"].toVariant().toBool());
        this->set_isEnabled(val["isEnabled"].toVariant().toBool());
        this->set_operators(val["operators"].toVariant().toString());
        this->set_clientType(val["clientType"].toVariant().toInt());
        this->set_createTime(val["createTime"].toVariant().toString());
        this->set_operateTime(val["operateTime"].toVariant().toString());
        this->set_updateFileId(val["updateFileId"].toVariant().toString());
        this->set_updateContent(val["updateContent"].toVariant().toString());
        ret = true;
    }
    return ret;
}

QByteArray UpdateVerQueryDto::toString()
{
    QJsonObject jsonParam;
    jsonParam.insert("id", id);
    jsonParam.insert("clientType", clientType);
    jsonParam.insert("version", version);
    jsonParam.insert("isEnabled", isEnabled);
    jsonParam.insert("isDeleted", isDeleted);
    jsonParam.insert("updateContent", updateContent);
    jsonParam.insert("updateFileId", updateFileId);
    jsonParam.insert("remark", remark);
    jsonParam.insert("operators", operators);
    jsonParam.insert("operateTime", operateTime);
    jsonParam.insert("creator", creator);
    jsonParam.insert("createTime", createTime);
    QJsonDocument doc;
    doc.setObject(jsonParam);
    QByteArray param = doc.toJson(QJsonDocument::Compact);
    return param;
}
