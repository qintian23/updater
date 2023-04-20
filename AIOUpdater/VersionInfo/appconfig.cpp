#include "appconfig.h"
#include "qsettings.h"
#include "qfile.h"

QString AppConfig::ConfigFile = "AIOConfig.ini";
QString AppConfig::ServerIp = "120.77.219.29";
QString AppConfig::ServerPort = "8080";
QString AppConfig::LogPath = "/usr/local/ITCUpdater/";
QString AppConfig::TargetAppName = "ITCBoard";
QString AppConfig::ITCBoard = "/usr/local/ITCBoard";// "/home/zhulc/AIOStarter/bin/starter.sh"; // "/usr/local/ITCBoard/ITCBoard.sh";
QString AppConfig::ItcBackupDir = "/usr/temp/data";
QString AppConfig::ItcDownloadDir = "/usr/temp/download";
QString AppConfig::UpdateDtoParam = "?ClientType=1&IsEnabled=true";

void AppConfig::readConfig()
{
    if (!checkConfig())
    {
        return;
    }

    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);
    set.beginGroup("BaseConfig");
    AppConfig::ServerIp = set.value("ServerIp", AppConfig::ServerIp).toString();
    AppConfig::ServerPort= set.value("ServerPort", AppConfig::ServerPort).toString();
    AppConfig::LogPath= set.value("LogPath", AppConfig::LogPath).toString();
    AppConfig::TargetAppName = set.value("TargetAppName", AppConfig::TargetAppName).toString();
    AppConfig::ITCBoard= set.value("ITCBoard", AppConfig::ITCBoard).toString();
    AppConfig::ItcBackupDir= set.value("ItcBackupDir", AppConfig::ItcBackupDir).toString();
    AppConfig::ItcDownloadDir= set.value("ItcDownloadDir", AppConfig::ItcDownloadDir).toString();
    AppConfig::UpdateDtoParam= set.value("UpdateDtoParam", AppConfig::UpdateDtoParam).toString();
    set.endGroup();
}

void AppConfig::writeConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);
    set.beginGroup("BaseConfig");
    set.setValue("ServerIp", AppConfig::ServerIp);
    set.setValue("ServerPort", AppConfig::ServerPort);
    set.setValue("LogPath", AppConfig::LogPath);
    set.setValue("TargetAppName", AppConfig::TargetAppName);
    set.setValue("ITCBoard", AppConfig::ITCBoard);
    set.setValue("ItcBackupDir", AppConfig::ItcBackupDir);
    set.setValue("ItcDownloadDir", AppConfig::ItcDownloadDir);
    set.setValue("UpdateDtoParam", AppConfig::UpdateDtoParam);
    set.sync();
    set.endGroup();
}

bool AppConfig::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(AppConfig::ConfigFile);
    if (file.size() == 0)
    {
        writeConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly))
    {
        bool ok = true;
        while (!file.atEnd())
        {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2)
            {
                if (list.at(1) == "")
                {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok)
        {
            writeConfig();
            return false;
        }
    }
    else
    {
        writeConfig();
        return false;
    }

    return true;
}
