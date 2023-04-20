#ifndef UPGRADEACCESS_H
#define UPGRADEACCESS_H

#include <QDebug>
#include <QString>

class UpgradeAccess
{
public:
    static UpgradeAccess*   instance        ();
    static bool             hasInstance     ();
    static void             releaseInstance ();

private:
    UpgradeAccess(){}

private:
    static UpgradeAccess* gInstance;

public:
    QString GetUrl(QString path, bool isHttps = false);

    QString QueryVerInfo(QString param);

    QString QueryFileInfo(QString paramJson);

    QString DeviceLogin();

    QString DeviceLogin(QString mac);

    QString obsFileInfo();

    QString obsFileLink(QString, QString);

    QString obsFileShare();
};

#endif // UPGRADEACCESS_H
