#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QUrl>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>


class jsonManager : public QObject
{
    Q_OBJECT
public:
    struct file
    {
        QString id;
        QString name;
        QString time;
        int size;
        QString type;
    };

    explicit jsonManager(QObject *parent = nullptr);
    void insertInt(QString jsonName,QList<QString> key,QList<int> value);
    void insertStr(QString jsonName,QList<QString> key,QList<QString> value);
    void insertObj(QString jsonName,file obj);
    void deleItem(QString jsonName,QString key);
    void clean(QString jsonName);
    QJsonObject getObj(QString jsonName);

private:
    QString m_jsonPath;

};

#endif // JSONMANAGER_H
