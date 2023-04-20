#include "externalappswitch.h"

#include <QDebug>

ExternalAppSwitch::ExternalAppSwitch()
{
    m_process = new QProcess();
}

ExternalAppSwitch::~ExternalAppSwitch()
{
    if(m_process)
    {
        delete m_process;
        m_process = nullptr;
    }
}

void ExternalAppSwitch::onApp(QString programPath, QStringList arguments)
{
    if(arguments.isEmpty())
         m_process->start(programPath);
    else
        m_process->start(programPath, arguments);
}

void ExternalAppSwitch::onApp(QString programPath)
{
    m_process->start(programPath);
}

void ExternalAppSwitch::openApp(QString programPath, QStringList arguments)
{
    bool isStarted = QProcess::startDetached(programPath, arguments);
    if(isStarted)
    {
        qDebug() << "-----------open success";
    }
    else
    {
        qDebug() << "-----------open faild";
    }
}

void ExternalAppSwitch::openApp(QString programPath)
{
//    const char *path = (programPath.toStdString()).c_str();
//    system(path);
    bool isStarted = QProcess::startDetached(programPath);
    if(isStarted)
    {
        qDebug() << "-----------open success";
    }
    else
    {
        qDebug() << "-----------open faild";
    }
}

void ExternalAppSwitch::closeApp()
{
    if(m_process)
    {
        m_process->kill();
    }
}

/**
#!/bin/sh
iwconfig 2>&1 | grep ESSID
exit $?
*/
void ExternalAppSwitch::controlAppByShell(QString programPath)
{
    m_process->start(programPath);
    m_process->waitForFinished();
    QString output = m_process->readAllStandardOutput();
    qDebug()<< output;
    QString err = m_process->readAllStandardError();
    qDebug()<< err;
}
