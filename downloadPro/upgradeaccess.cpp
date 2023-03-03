#include "upgradeaccess.h"
#include "appconfig.h"
#include <QObject>
#include <QDebug>

upgradeaccess* upgradeaccess::gInstance = 0;

upgradeaccess* upgradeaccess::instance()
{
    if(!gInstance)
        gInstance = new upgradeaccess;
    return  gInstance;
}

bool upgradeaccess::hasInstance()
{
    return gInstance != nullptr;
}

void upgradeaccess::releaseInstance()
{
    if(gInstance)
        delete gInstance;
    gInstance = nullptr;
}

QString upgradeaccess::GetUrl(QString path, bool isHttps)
{
    QString httpStr = isHttps ? "https" : "http";
    QString url = QObject::tr("%1://%2:%3%4").arg(httpStr)
            .arg(AppSetting::ServerIp)
            .arg(AppSetting::ServerPort)
            .arg(path);
    qDebug() << url;
    return url;
}

QString upgradeaccess::QueryVerInfo(QString param)
{
    QString queryParam = QObject::tr("/api/internal/SysUpdateVerMgr/VerList%1").arg(param);
    QString tmpUrl = GetUrl(queryParam);
    qDebug() << tmpUrl;
    return tmpUrl;
}

QString upgradeaccess::QueryFileInfo(QString paramJson)
{
    QString queryParam = QObject::tr("/api/internal/SysFile/QuerySysFileInfo%1").arg(paramJson);
    QString tmpUrl = GetUrl(queryParam);
    qDebug() << tmpUrl;
    // to do
    return tmpUrl;
}

bool upgradeaccess::DownloadFileById(QString fileId, QString savePath, QString fileName)
{
    QString fileInfo = GetUrl("/oss/fileInfo"); // +josn
    QString tmpfilelinkUrl = QObject::tr("/oss/fileLink?fileName=%1%2")
            .arg("fileInfo.Data.FilePath")
            .arg("fileInfo.Data.FileObjectName}"); // json
    QString fileLink = GetUrl(tmpfilelinkUrl);
    return true;
}
