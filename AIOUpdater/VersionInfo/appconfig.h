#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

class AppConfig
{
public:
    static QString ConfigFile;
    static QString TargetAppName;
    static QString ServerIp;
    static QString ServerPort;
    static QString LogPath;
    static QString ITCBoard;
    static QString ItcBackupDir;
    static QString ItcDownloadDir;
    static QString UpdateDtoParam;

public:
    static void readConfig();
    static void writeConfig();
    static bool checkConfig();
};


#endif // APPCONFIG_H
