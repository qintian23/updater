#include "aioupdater.h"
#include "VersionInfo/zjsontools.hpp"
#include "VersionInfo/upgradeaccess.h"
#include "VersionInfo/httpcommon.h"
#include "VersionInfo/appconfig.h"
#include "Logging/logmanager.h"
#include "Logging/updatelog.h"
#include "Updater/rollbackmechanism.h"
#include "Downloader/jsonManager.h"

#include <QTimer>

AIOUpdater::AIOUpdater(QObject *parent) : QObject(parent)
{
    u_Dialog = new UpdaterDialog();
    m_appSwitch = new ExternalAppSwitch();

    unzipFile = new CompreDecompreFileThread();

    u_Dialog->show();
    m_status = true;
    qDebug() << "OpenSSL Support: " << QSslSocket::supportsSsl();
    connect(this,&AIOUpdater::decompress_signal,unzipFile,&CompreDecompreFileThread::decompression_file_run);
    connect(unzipFile,&CompreDecompreFileThread::decompression_res_signal,this,&AIOUpdater::decompress_res_slot);
    connect(u_Dialog, SIGNAL(fileDownloadStatu(bool)), this, SLOT(downloadFileFinished(bool)));

    QObject::connect(HttpCommon::instance(), &HttpCommon::httpPostRspReady, [&](QString url, QString postMD5, QByteArray d){
       qDebug()<<postMD5;
       if(url != nullptr && d != nullptr)
       {
           if(url.contains("/sysDevice/deviceLogin/"))
           {
               setLoginInfo(d);

           }
           else if(url.contains("/oss/fileInfo"))
           {
                setObsFileInfo(d);
           }
           else if(url.contains("/oss/fileLink"))
           {
               setObsLinkInfo(d);
//               ZJsonTools::Print(d);
           }
           else if(url.contains("/sysFile/Share"))
           {
//               ZJsonTools::Print(d);
               setObsLinkInfo(d);
           }
       }
    });

    QObject::connect(HttpCommon::instance(), &HttpCommon::httpGetRspReady, [&](QString url, QByteArray d){
       if(url != nullptr && d != nullptr)
       {
           if(url.contains("/SysUpdateVerMgr/VerList"))
           {
//                ZJsonTools::Print(d);
                setVerDto(d);
           }
           else if(url.contains("/SysFile/QuerySysFileInfo/"))
           {
                setSysFileInfo(d);
           }
       }
    });
}

AIOUpdater::~AIOUpdater()
{
    delete u_Dialog;
    u_Dialog = nullptr;

    delete m_appSwitch;
    m_appSwitch = nullptr;

    delete unzipFile;
    unzipFile = nullptr;
}

// record update process
void AIOUpdater::update()
{
//    QByteArray deviceMac("mac=%3A566AE841-A3A3-32BB-FFA6-58112204B271");
//    jsonManager mana;
//    QString strMacAddr = mana.getObj("code.json").value(("macID")).toString();
//    onDeviceLogin(strMacAddr);
//    qDebug() << "***************** strMacAddr = " << strMacAddr;
    GetVerDto("?ClientType=1&IsEnabled=true");
}

void AIOUpdater::onDeviceLogin(QByteArray deviceMac)
{
    QString url = UpgradeAccess::instance()->DeviceLogin();
    HttpCommon::instance()->httpPost(url, deviceMac);
}

void AIOUpdater::onDeviceLogin(QString mac)
{
    QString url = UpgradeAccess::instance()->DeviceLogin(mac);
    HttpCommon::instance()->httpPost(url, mac.toLatin1());
}

void AIOUpdater::GetObsFileInfo(QByteArray param)
{
    QString url = UpgradeAccess::instance()->obsFileInfo();

    HttpCommon::instance()->httpPost(url, param, 1);
}

void AIOUpdater::GetVerDto(QString param)
{
    QString url = UpgradeAccess::instance()->QueryVerInfo(param);
    HttpCommon::instance()->httpGet(url);
}

void AIOUpdater::GetSysFileInfo(QString param)
{
    QString url = UpgradeAccess::instance()->QueryFileInfo(param);
    HttpCommon::instance()->httpGet(url);
}

void AIOUpdater::GetObsLinkInfo(QString p1, QString p2)
{
    QString url = UpgradeAccess::instance()->obsFileLink(p1, p2);
    HttpCommon::instance()->httpPost(url, "", 1);
}

void AIOUpdater::GetobsShareInfo(QString id)
{
    if(!id.isNull() || id.length() >0)
    {
        UpdateLog::UpdateTime = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]");
        QString url = UpgradeAccess::instance()->obsFileShare();
        QJsonObject jsonParam;
        jsonParam.insert("id", id);
        QJsonDocument doc;
        doc.setObject(jsonParam);
        QByteArray param = doc.toJson(QJsonDocument::Compact);
        qDebug() << "===========" << param;
        HttpCommon::instance()->httpPost(url, param, 1);
    }
}

// download the files of update
void AIOUpdater::onDownload()
{
    RollbackMechanism::Deletefolder(AppConfig::ItcDownloadDir);
    if(m_status)
    {
        QString url = obs_link.get_data();
        u_Dialog->onDownload(url, AppConfig::ItcDownloadDir);
    }
}

// back up the files that will update
void AIOUpdater::startBackup()
{
    QString filePath = AppConfig::ITCBoard;
    bool ret = RollbackMechanism::copyDirectory(filePath, AppConfig::ItcBackupDir, true);
    if(ret)
    {
        qDebug().noquote() << tr("Info, backup success<%1>...\n").arg(0);
        UpdateLog::StepBackUp = true;
    }
    else
    {
        qCritical().noquote() << tr("Info, backup failed<%1>...\n").arg(0);
        UpdateLog::StepBackUp = false;
    }
}

// decompre file and copy the file to the target path
void AIOUpdater::decompreFile()
{
    QString filePath = AppConfig::ItcDownloadDir +"/" + QUrl(obs_link.get_data()).fileName();
    QString zipPath = AppConfig::ITCBoard;
    int ret = (unzipFile->Unzip(filePath, zipPath)) ? 100 : 0;
    qInfo().noquote() << tr("Info, decompress success<%1>...\n").arg(ret);
    UpdateLog::StepDecompress = ret == 100;
    qInfo().noquote() << tr("Info, update success<%1>...\n").arg(ret);
    UpdateLog::StepUpdate = ret == 100;
    uProcessSwitch();
}

// the switch of process
void AIOUpdater::uProcessSwitch()
{
    UpdateLog::writeConfig();
//    m_appSwitch->openApp(AppConfig::ITCBoard);
//    system(AppConfig::ITCBoard.toStdString().c_str());
    QString cmd = QString("chmod 777 %1/%2").arg(AppConfig::ITCBoard).arg(AppConfig::TargetAppName);
    system(cmd.toStdString().c_str());
    qDebug() << "--------start itcboard";
    u_Dialog->close();
    // to do
}

void AIOUpdater::downloadFileFinished(bool statu)
{
    if(statu)
    {
        startBackup();
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        qDebug() << "====wait decompress====";
        loop.exec();
        qDebug() << "====start decompress====";
        decompreFile();
    }
    else
    {
        uProcessSwitch();
    }
}

void AIOUpdater::decompress_res_slot(CompreDecompreFileThread::ResultE res, quint8 progress, QString remarks)
{
    qDebug()<<"deCom:"<<"  res:"<<res<<"  progress:"<<progress<<"  remarks:"<<remarks;
}

void AIOUpdater::setSysFileInfo(QByteArray responce)
{
    bool ret = u_file.init(responce);
    if(ret)
    {
        QJsonObject jsonParam;
        jsonParam.insert("id", u_file.get_id());
        QJsonDocument doc;
        doc.setObject(jsonParam);
        QByteArray param = doc.toJson(QJsonDocument::Compact);
        GetObsFileInfo(param);
    }
    else
    {
        qDebug() << "get share info error";
    }
}

void AIOUpdater::setVerDto(QByteArray responce)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(responce, &jsonError);
    if(!document.isNull() && (QJsonParseError::NoError == jsonError.error))
    {
       QJsonObject obj = document.object();
       if(obj.contains("data"))
       {
            QJsonArray childArray = obj.take("data").toArray();
            int i = 0;
            for(auto iter = childArray.begin(); iter!=childArray.end(); ++iter)
            {
                QJsonObject val = (*iter).toObject();

                UpdateVerQueryDto u_file = UpdateVerQueryDto();
                bool ret = u_file.init(val);
                if(ret)
                {
                   u_verDto.insert(i++, u_file);
                }
            }
       }
    }
    if(u_verDto.size() > 0)
        GetobsShareInfo(u_verDto[u_verDto.size()-1].get_updateFileId());
//    GetSysFileInfo(u_verDto[0].get_updateFileId());
}

void AIOUpdater::setObsFileInfo(QByteArray responce)
{
    if(obs_file.init(responce))
    {
        GetObsLinkInfo(obs_file.get_filePath(), obs_file.get_fileObjectName());
    }
    else
    {
        qDebug() << "get obs file error";
    }
}

void AIOUpdater::setObsLinkInfo(QByteArray responce)
{
    if(obs_link.init(responce))
    {
        if(obs_link.get_data().isNull() || !obs_link.get_success())
        {
            u_Dialog->SetStatus(obs_link.get_message());
        }
        else
        {
            onDownload();
        }
    }
    else
    {
        qDebug() << "get obs link info error";
    }
}

void AIOUpdater::setLoginInfo(QByteArray responce)
{
    if(u_login.init(responce))
    {
        if(u_login.get_code()==200)
        {
            GetVerDto("?ClientType=1&IsEnabled=true");
        }
        else // if(u_login.get_code()==400)
        {
            u_Dialog->SetStatus(u_login.get_message());
            qDebug()<< u_login.get_message();
        }
    }
    else
    {
        qDebug() << "login error";
    }

}
