#ifndef AIOUPDATER_H
#define AIOUPDATER_H

#include "Ui/updaterdialog.h"
#include "Downloader/httpdownload.h"
#include "VersionInfo/sysfile.h"
#include "VersionInfo/obslinkdto.h"
#include "VersionInfo/updateverquerydto.h"
#include "Updater/externalappswitch.h"
#include "Updater/compredecomprefilethread.h"

#include <QObject>

#define ATTRIBUTE_GET_FUNC(argType, arg)\
    public:\
    argType get_##arg(){\
        return arg;\
    }\

class AIOUpdater : public QObject
{
    Q_OBJECT
public:
    explicit AIOUpdater(QObject *parent = nullptr);
//    AIOUpdater();
    virtual ~AIOUpdater();

private:
    UpdaterDialog *u_Dialog;
    ExternalAppSwitch *m_appSwitch;

    qint64 m_bytes;
    bool m_status;

    SysFile u_file;
    SysFile obs_file;
    obsLinkDto obs_link;
    obsLinkDto u_login;
    QList<UpdateVerQueryDto> u_verDto;

    CompreDecompreFileThread *unzipFile;


signals:
    void decompress_signal(QString srcPath,QString destPath);

private slots:
    void decompress_res_slot(CompreDecompreFileThread::ResultE res,quint8 progress,QString remarks = nullptr);

    void downloadFileFinished(bool);

public:
    ATTRIBUTE_GET_FUNC(SysFile, u_file);
    ATTRIBUTE_GET_FUNC(QList<UpdateVerQueryDto>, u_verDto);

public:
    // download the files of update
    void onDownload();
    // back up the files that will update
    void startBackup();
    // record update process
    void update();
    // decompre file and copy the file to the target path
    void decompreFile();
    // the switch of process
    void uProcessSwitch();

private:
    void onDeviceLogin(QByteArray);
    void onDeviceLogin(QString);
    void GetSysFileInfo(QString param);
    void GetVerDto(QString param);
    void setSysFileInfo(QByteArray);
    void setVerDto(QByteArray responce);
    void GetObsFileInfo(QByteArray);
    void setObsFileInfo(QByteArray);
    void GetObsLinkInfo(QString, QString);
    void setObsLinkInfo(QByteArray);
    void GetobsShareInfo(QString);
    void setLoginInfo(QByteArray);
};

#endif // AIOUPDATER_H
