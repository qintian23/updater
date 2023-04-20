#ifndef EXTERNALAPPSWITCH_H
#define EXTERNALAPPSWITCH_H

#include <QString>
#include <QStringList>
#include <QProcess>

class ExternalAppSwitch
{
public:
    ExternalAppSwitch();
    virtual ~ExternalAppSwitch();

private:
    QProcess *m_process;

public:
    void onApp(QString programPath);
    void onApp(QString programPath, QStringList arguments);
    void controlAppByShell(QString);
    void openApp(QString programPath, QStringList arguments);
    void openApp(QString programPath);
    void closeApp();
};

#endif // EXTERNALAPPSWITCH_H
