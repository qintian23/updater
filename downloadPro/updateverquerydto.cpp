#include "updateverquerydto.h"
#include "upgradeaccess.h"
#include "chttpsupport.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>

UpdateVerQueryDto::UpdateVerQueryDto():
    ClientType(1),
    Version(""),
    IsEnabled(false)
{
    GetUpdateVersion();
}

void UpdateVerQueryDto::GetUpdateVersion()
{
    QObject::connect(chttpsupport::instance(), &chttpsupport::httpGetRspReady, [&](QString url, QByteArray d){
            if(d != nullptr && url != nullptr)
            {
//                qDebug() << d;
                SetUpdateInfo(d);
            }
        });
    QString url = upgradeaccess::instance()->QueryVerInfo("?ClientType=1&IsEnabled=true");
    chttpsupport::instance()->httpGet(url);
}

void UpdateVerQueryDto::SetUpdateInfo(QByteArray responce)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(responce, &jsonError);
    if(!document.isNull() && (QJsonParseError::NoError == jsonError.error))
    {
        if(document.isObject())
        {
            QJsonObject obj = document.object();
            if(obj.contains("code"))
            {
                QJsonValue value = obj.take("code");
                if(value != 200)
                {
                    return;
                }
            }

            if(obj.contains("data"))
            {
                QJsonValue data = obj.take("data");
                if(data.isObject())
                {
                    QJsonObject dataObj = data.toObject();

                    if(dataObj.contains("version"))
                    {
                        QJsonValue ver = dataObj.take("version");
                        if(ver.isString())
                        {
                            QString version = ver.toVariant().toString();
                            SetVersion(version);
                        }
                    }

                    if(dataObj.contains("clientType"))
                    {
                        QJsonValue tmp = dataObj.take("clientType");
                        if(tmp.isDouble())
                        {
                            int temp = tmp.toVariant().toUInt();
                            SetClientType(temp);
                        }
                    }

                    if(dataObj.contains("isEnabled"))
                    {
                        QJsonValue tmp = dataObj.take("isEnabled");
                        if(tmp.isBool())
                        {
                            bool temp = tmp.toVariant().toBool();
                            SetIsEnabled(temp);
                        }
                    }

                }
            }
        }
    }
}

void UpdateVerQueryDto::SetClientType(int type)
{
    ClientType = type;
    qDebug() << ClientType;
}

void UpdateVerQueryDto::SetVersion(QString ver)
{
    Version = ver;
    qDebug() << Version;
}

void UpdateVerQueryDto::SetIsEnabled(bool isEnabled)
{
    IsEnabled = isEnabled;
    qDebug() << IsEnabled;
}
