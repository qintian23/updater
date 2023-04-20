#include "jsonManager.h"

jsonManager::jsonManager(QObject *parent) : QObject(parent)
{
    QString curfileName = QDir::homePath() + QString("/.config/ITC");
    //QCoreApplication::applicationDirPath();
    m_jsonPath = curfileName ;
}

QJsonObject jsonManager::getObj(QString jsonName)
{
    QFile file(m_jsonPath+"/"+jsonName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() <<__FUNCTION__<< "File read failed!";
    } else {
        //        qDebug() <<__FUNCTION__<<"File read successfully!";
    }
    QByteArray allData = file.readAll();
    file.close();
    QJsonParseError json_error;
    //    qDebug()<<__FUNCTION__<<"arrsize"<<allData.size();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        //        qDebug() <<__FUNCTION__<< "json error!";
        return QJsonObject();
    }
    else return jsonDoc.object();
}

void jsonManager::clean(QString jsonName)
{
    QFile file(m_jsonPath+"/"+jsonName);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置写入编码是UTF8
    // 写入文件
    stream << "{}";
    file.close();
}


void jsonManager::deleItem(QString jsonName,QString key)
{
    QFile file(m_jsonPath+"/"+jsonName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() <<__FUNCTION__<< "File read failed!";
    } else {
        qDebug() <<__FUNCTION__<<"File read successfully!";
    }
    QByteArray allData = file.readAll();
    file.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() <<__FUNCTION__<< "json error!";
        return;
    }
    QJsonObject rootObj=jsonDoc.object();
    if(rootObj.contains(key)) rootObj.remove(key);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() <<__FUNCTION__<< "can't open error!";
    }
    else{
        QJsonDocument document;
        document.setObject(rootObj);
//        qDebug()<<__FUNCTION__<<"document-deleItem"<<document;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");		// 设置写入编码是UTF8
        // 写入文件
        stream << document.toJson();
        file.close();
    }
}

bool isFileExist(QString fullFileName)
{
    QFile file(fullFileName);
    if(file.exists())
    {
        return true;
    }
    else
    {
        qDebug() <<__FUNCTION__<< "文件不存在, 新建该文件";
        file.open( QIODevice::ReadWrite | QIODevice::Text );
        //注意关闭文件
        QTextStream stream(&file);
        stream.setCodec("UTF-8");		// 设置写入编码是UTF8
        // 写入文件
        stream << "{}";
        file.close();
    }
    return false;
}


void jsonManager::insertStr(QString jsonName,QList<QString> key,QList<QString> value)
{
    qDebug()<<__FUNCTION__<<"[JsonManager]insertStr";
    QString strKey;
    QString strValue;

    isFileExist(m_jsonPath+"/"+jsonName);

    QFile file(m_jsonPath+"/"+jsonName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() <<__FUNCTION__<< "[JsonManager]File read failed!";
    } else {
        qDebug() <<__FUNCTION__<<"[JsonManager]File read successfully!";
    }

    QByteArray allData = file.readAll();
    file.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() <<__FUNCTION__<< "insert-json error!"<<allData<<json_error.errorString();
        return;
    }
    QJsonObject rootObj=jsonDoc.object();
    if(rootObj.contains(key.at(0)))
    {
        for(int i=0;i<key.size();i++)
        {
            rootObj[key.at(i)]=value.at(i);
        }
    }
    else
    {
        for(int i=0;i<value.size();i++)
        {
            rootObj.insert(key.at(i),value.at(i));
        }
    }
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() <<__FUNCTION__<< "can't write error!";
    }
    else{
        QJsonDocument document;
        document.setObject(rootObj);
//        qDebug()<<__FUNCTION__<<"document-insertStr"<<document;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");		// 设置写入编码是UTF8
        // 写入文件
        stream << document.toJson();
        file.close();
    }
}


void jsonManager::insertInt(QString jsonName,QList<QString> key,QList<int> value)
{
    isFileExist(m_jsonPath+"/"+jsonName);
    QFile file(m_jsonPath+"/"+jsonName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() <<__FUNCTION__<< "File read failed!";
    } else {
        qDebug() <<__FUNCTION__<<"File read successfully!";
    }
    QByteArray allData = file.readAll();
    file.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() <<__FUNCTION__<< "json error!";
        return;
    }
    QJsonObject rootObj=jsonDoc.object();
    if(rootObj.contains(key.at(0)))
    {
        for(int i=0;i<key.size();i++)
        {
            rootObj[key.at(i)]=value.at(i);
        }
    }
    else
    {
        for(int i=0;i<value.size();i++)
        {
            rootObj.insert(key.at(i),value.at(i));
        }
    }
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() <<__FUNCTION__<< "can't open error!";
    }
    else{
        QJsonDocument document;
        document.setObject(rootObj);
//        qDebug()<<__FUNCTION__<<"document-insertInt"<<document;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");		// 设置写入编码是UTF8
        // 写入文件
        stream << document.toJson();
        file.close();
    }
}

void jsonManager::insertObj(QString jsonName,file obj)
{
    isFileExist(m_jsonPath+"/"+jsonName);
    QFile file(m_jsonPath+"/"+jsonName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() <<__FUNCTION__<< "File read failed!";
    } else {
        qDebug() <<__FUNCTION__<<"File read successfully!";
    }
    QByteArray allData = file.readAll();
    file.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() <<__FUNCTION__<< "json error!";
        return;
    }
    QJsonObject rootObj=jsonDoc.object();
    QJsonObject sub;
    sub.insert("name",obj.name);
    sub.insert("time",obj.time);
    sub.insert("size",obj.size);
    sub.insert("type",obj.type);
    rootObj.insert(obj.id,sub);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() <<__FUNCTION__<< "can't open error!";
    }
    else{
        QJsonDocument document;
        document.setObject(rootObj);
//        qDebug()<<__FUNCTION__<<"document-insertInt"<<document;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");		// 设置写入编码是UTF8
        // 写入文件
        stream << document.toJson();
        file.close();
    }
}


