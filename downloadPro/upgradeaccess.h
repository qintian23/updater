#ifndef UPGRADEACCESS_H
#define UPGRADEACCESS_H

#include <QString>
#include <QDebug>

class upgradeaccess
{
public:
    static upgradeaccess*   instance        ();
    static bool             hasInstance     ();
    static void             releaseInstance ();

private:
    upgradeaccess(){}

private:
    static upgradeaccess* gInstance;

//    class GC
//    {
//    public:
//        ~GC()
//        {
//            if(gInstance != nullptr)
//            {
//                qDebug() << "Here destroy the gInstance...";
//                delete gInstance;
//            }
//            gInstance = nullptr;
//        }
//        static GC gc;
//    };

public:
    QString GetUrl(QString path, bool isHttps = false);

    QString QueryVerInfo(QString param);

    QString QueryFileInfo(QString paramJson);

    bool DownloadFileById(QString fileId, QString savePath, QString fileName);
};

#endif // UPGRADEACCESS_H
