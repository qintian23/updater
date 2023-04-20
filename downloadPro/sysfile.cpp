#include "sysfile.h"
#include "chttpsupport.h"
#include "upgradeaccess.h"
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>

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
    InitFileInfo("/path");
}

void SysFile::InitFileInfo(QString param)
{
    QObject::connect(chttpsupport::instance(), &chttpsupport::httpGetRspReady, [&](QString url, QByteArray d){
       if(url != nullptr && d != nullptr)
       {
           qDebug() << d;
           setFileInfo(d);
       }
    });
    QString url = upgradeaccess::instance()->QueryFileInfo(param);
    chttpsupport::instance()->httpGet(url);
}

void SysFile::setFileInfo(QByteArray responce)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(responce, &jsonError);
    if(!document.isNull() && (QJsonParseError::NoError == jsonError.error))
    {
        if(document.isObject())
        {
            QJsonObject obj = document.object();
            if(obj.contains("success"))
            {
                QJsonValue value = obj.take("success");
                if(value.isBool())
                {
                    bool tmp = value.toVariant().toBool();
                    if(tmp)
                    {
                        if(obj.contains("data"))
                        {
                            QJsonValue data = obj.take("data");
                            if(data.isObject())
                            {
                                QJsonObject dataObj = data.toObject();

                                if(dataObj.contains("id"))
                                {
                                    QJsonValue tmp = dataObj.take("id");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_id(temp);
                                    }
                                }

                                if(dataObj.contains("fileLocation"))
                                {
                                    QJsonValue tmp = dataObj.take("fileLocation");
                                    if(tmp.isDouble())
                                    {
                                        int temp = tmp.toVariant().toInt();
                                        this->set_fileLocation((OSSProvider)temp);
                                    }
                                }

                                if(dataObj.contains("bucketName"))
                                {
                                    QJsonValue tmp = dataObj.take("bucketName");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_bucketName(temp);
                                    }
                                }

                                if(dataObj.contains("fileOriginName"))
                                {
                                    QJsonValue tmp = dataObj.take("fileOriginName");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_fileOriginName(temp);
                                    }
                                }

                                if(dataObj.contains("fileObjectName"))
                                {
                                    QJsonValue tmp = dataObj.take("fileObjectName");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_fileObjectName(temp);
                                    }
                                }

                                if(dataObj.contains("suffix"))
                                {
                                    QJsonValue tmp = dataObj.take("suffix");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_suffix(temp);
                                    }
                                }

                                if(dataObj.contains("filePath"))
                                {
                                    QJsonValue tmp = dataObj.take("filePath");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_filePath(temp);
                                    }
                                }

                                if(dataObj.contains("sizeKb"))
                                {
                                    QJsonValue tmp = dataObj.take("sizeKb");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_sizeKb(temp);
                                    }
                                }

                                if(dataObj.contains("sizeInfo"))
                                {
                                    QJsonValue tmp = dataObj.take("sizeInfo");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_sizeInfo(temp);
                                    }
                                }

                                if(dataObj.contains("md5"))
                                {
                                    QJsonValue tmp = dataObj.take("md5");
                                    if(tmp.isString())
                                    {
                                        QString temp = tmp.toVariant().toString();
                                        this->set_md5(temp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
