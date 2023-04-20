#ifndef UPDATELOG_H
#define UPDATELOG_H

#include <QStringList>

class UpdateLog
{
public:
    static QString UpdateTime;
    static QString UpdateMessage;
    static QString ConfigFile;
    static bool StepDownload;
    static bool StepBackUp;
    static bool StepDecompress;
    static bool StepUpdate;

    static void readConfig();
    static void writeConfig();
    static bool checkConfig();
};

#endif // UPDATELOG_H
