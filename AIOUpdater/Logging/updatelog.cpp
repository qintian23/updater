#include "updatelog.h"
#include "qsettings.h"
#include "qfile.h"

QString UpdateLog::UpdateTime = "2023-03-20 09:29:19";
QString UpdateLog::UpdateMessage = "test update";
QString UpdateLog::ConfigFile = "UpdateLog.ini";
bool UpdateLog::StepDownload = false;
bool UpdateLog::StepBackUp = false;
bool UpdateLog::StepDecompress = false;
bool UpdateLog::StepUpdate = false;

void UpdateLog::readConfig()
{
    if (!checkConfig())
    {
        return;
    }

    QSettings set(UpdateLog::ConfigFile, QSettings::IniFormat);
    set.beginGroup("BaseConfig");
    UpdateLog::UpdateTime = set.value("UpdateTime", UpdateLog::UpdateTime).toString();
    UpdateLog::UpdateMessage = set.value("UpdateMessage", UpdateLog::UpdateMessage).toString();
    UpdateLog::ConfigFile = set.value("ConfigFile", UpdateLog::ConfigFile).toString();
    UpdateLog::StepDownload = set.value("StepDownload", UpdateLog::StepDownload).toBool();
    UpdateLog::StepBackUp = set.value("StepBackUp", UpdateLog::StepBackUp).toBool();
    UpdateLog::StepDecompress = set.value("StepDecompress", UpdateLog::StepDecompress).toBool();
    UpdateLog::StepUpdate = set.value("StepUpdate", UpdateLog::StepUpdate).toBool();
    set.endGroup();
}

void UpdateLog::writeConfig()
{
    QSettings set(UpdateLog::ConfigFile, QSettings::IniFormat);
    set.beginGroup("BaseConfig");
    set.setValue("UpdateTime", UpdateLog::UpdateTime);
    set.setValue("UpdateMessage", UpdateLog::UpdateMessage);
    set.setValue("ConfigFile", UpdateLog::ConfigFile);
    set.setValue("StepDownload", UpdateLog::StepDownload);
    set.setValue("StepBackUp", UpdateLog::StepBackUp);
    set.setValue("StepDecompress", UpdateLog::StepDecompress);
    set.setValue("StepUpdate", UpdateLog::StepUpdate);
    set.sync();
    set.endGroup();
}

bool UpdateLog::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(UpdateLog::ConfigFile);
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
