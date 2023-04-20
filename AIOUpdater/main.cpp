#include "aioupdater.h"
#include "VersionInfo/appconfig.h"
#include "Logging/logmanager.h"
#include "Logging/updatelog.h"
#include <QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    LogManager::getInstance()->initManager();//初始化

    QApplication a(argc, argv);

    AppConfig::ConfigFile = qApp->applicationDirPath() + "/AIOConfig.ini";
    AppConfig::readConfig();

    UpdateLog::ConfigFile = qApp->applicationDirPath() + "/UpdateLog.ini";
    UpdateLog::readConfig();

    AIOUpdater *updater = new AIOUpdater();
    updater->update();
    return a.exec();
}
