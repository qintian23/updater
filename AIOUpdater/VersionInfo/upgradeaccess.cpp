#include "upgradeaccess.h"
#include "appconfig.h"
#include <QObject>
#include <QDebug>

UpgradeAccess* UpgradeAccess::gInstance = 0;

UpgradeAccess* UpgradeAccess::instance()
{
    if(!gInstance)
        gInstance = new UpgradeAccess;
    return  gInstance;
}

bool UpgradeAccess::hasInstance()
{
    return gInstance != nullptr;
}

void UpgradeAccess::releaseInstance()
{
    if(gInstance)
        delete gInstance;
    gInstance = nullptr;
}

QString UpgradeAccess::GetUrl(QString path, bool isHttps)
{
    QString httpStr = isHttps ? "https" : "http";
    QString url = QObject::tr("%1://%2:%3%4").arg(httpStr)
            .arg(AppConfig::ServerIp)
            .arg(AppConfig::ServerPort)
            .arg(path);
    qDebug() << url;
    return url;
}

QString UpgradeAccess::QueryVerInfo(QString param)
{
    QString queryParam = QObject::tr("/api/internal/SysUpdateVerMgr/VerList%1").arg(param);
    QString tmpUrl = GetUrl(queryParam);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString UpgradeAccess::QueryFileInfo(QString paramJson)
{
    QString queryParam = QObject::tr("/api/internal/SysFile/QuerySysFileInfo/%1").arg(paramJson);
    QString tmpUrl = GetUrl(queryParam);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString UpgradeAccess::DeviceLogin()
{
    QString loginParam = "/api/internal/sysDevice/deviceLogin/%1";
    QString tmpUrl = GetUrl(loginParam);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString UpgradeAccess::DeviceLogin(QString mac)
{
    QString loginParam = QString("/api/internal/sysDevice/deviceLogin/%1").arg(mac);
    QString tmpUrl = GetUrl(loginParam);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString UpgradeAccess::obsFileInfo()
{
    QString param = "/oss/fileInfo";
    QString tmpUrl = GetUrl(param);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString UpgradeAccess::obsFileLink(QString filePath, QString fileObjName)
{
    QString param = QString("/oss/fileLink?fileName=%1/%2").arg(filePath).arg(fileObjName);
    QString tmpUrl = GetUrl(param);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString UpgradeAccess::obsFileShare()
{
    QString param = QString("/sysFile/Share");
    QString tmpUrl = GetUrl(param);
    qDebug() << tmpUrl;
    return tmpUrl;
}
