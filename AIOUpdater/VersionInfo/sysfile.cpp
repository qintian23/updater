#include "sysfile.h"
#include "httpcommon.h"
#include "upgradeaccess.h"
#include "zjsontools.hpp"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

SysFile::SysFile()
    : id("")
    , fileLocation(OSSProvider::Aliyun)
    , bucketName("")
    , fileOriginName("")
    , fileObjectName("")
    , suffix("")
    , filePath("")
    , sizeKb("")
    , sizeInfo("")
    , md5("")
{
    createTime = "";
    updateTime = "";
    createdUserId = "";
    createdUesrName = "";
    updatedUserId = "";
    updatedUserName = "";
    isDelete = "";
}

bool SysFile::init(QByteArray responce)
{
    bool ret = false;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(responce, &jsonError);
    if(!document.isNull() && (QJsonParseError::NoError == jsonError.error))
    {
       QJsonObject obj = document.object();
       if(obj.contains("data"))
       {
            QJsonObject val = obj.take("data").toObject();
            this->set_id(val["id"].toVariant().toString());
            this->set_md5(val["md5"].toVariant().toString());
            this->set_sizeKb(val["sizeKb"].toVariant().toString());
            this->set_suffix(val["suffix"].toVariant().toString());
            this->set_filePath(val["filePath"].toVariant().toString());
            this->set_sizeInfo(val["sizeInfo"].toVariant().toString());
            this->set_bucketName(val["bucketName"].toVariant().toString());
            this->set_fileLocation((OSSProvider)val["sizeKb"].toVariant().toInt());
            this->set_fileObjectName(val["fileObjectName"].toVariant().toString());
            this->set_fileOriginName(val["fileOriginName"].toVariant().toString());
            this->set_createTime(val["createTime"].toVariant().toString());
            this->set_updateTime(val["updateTime"].toVariant().toString());
            this->set_createdUserId(val["createdUserId"].toVariant().toString());
            this->set_createdUesrName(val["createdUesrName"].toVariant().toString());
            this->set_updatedUserId(val["updatedUserId"].toVariant().toString());
            this->set_updatedUserName(val["updatedUserName"].toVariant().toString());
            this->set_isDelete(val["isDelete"].toVariant().toInt());
            ret = true;
       }
    }
    return ret;
}

QByteArray SysFile::toString()
{
    QJsonObject jsonParam;
    jsonParam.insert("id", id);
    jsonParam.insert("fileLocation", fileLocation);
    jsonParam.insert("bucketName", bucketName);
    jsonParam.insert("fileOriginName", fileOriginName);
    jsonParam.insert("fileObjectName", fileObjectName);
    jsonParam.insert("suffix", suffix);
    jsonParam.insert("filePath", filePath);
    jsonParam.insert("sizeKb", sizeKb);
    jsonParam.insert("sizeInfo", sizeInfo);
    jsonParam.insert("md5", md5);
    jsonParam.insert("createTime", createTime);
    jsonParam.insert("updateTime", updateTime);
    jsonParam.insert("createdUserId", createdUserId);
    jsonParam.insert("createdUesrName", createdUesrName);
    jsonParam.insert("updatedUserId", updatedUserId);
    jsonParam.insert("updatedUserName", updatedUserName);
    jsonParam.insert("isDelete", isDelete);
    QJsonDocument doc;
    doc.setObject(jsonParam);
    QByteArray param = doc.toJson(QJsonDocument::Compact);
    return param;
}
