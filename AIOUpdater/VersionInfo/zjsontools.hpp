#ifndef ZJSONTOOLS_HPP
#define ZJSONTOOLS_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QList>
#include <QDebug>

class ZJsonTools final
{
private:
    ZJsonTools();

public:

    static void Print(QByteArray json)
    {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(json, &jsonError);
        PrintJson(document);
    }

    static void LoadJsonFromFile(const QString& jsonfilename,QJsonDocument& jsDoc)
    {
        QFile file(jsonfilename);
        file.open(QIODevice::ReadWrite);
        QByteArray json = file.readAll();
        jsDoc = QJsonDocument::fromJson(json);
    }

    static void PrintJson(QJsonDocument& jDoc)
    {
        if(jDoc.isObject())
        {
            QJsonObject jObject = jDoc.object();
            PrintJsonObject(jObject,"根");
        }
        else if(jDoc.isArray())
        {
            QJsonArray jArray = jDoc.array();
            PrintJsonArray(jArray,"");
        }
        else
        {
            //空文档
            qDebug() << "空文档";
        }
    }

    static bool SearchJsonValue(const QJsonObject& jObject,const QStringList& path,QJsonValue& result)
    {
        QJsonValue jValue = jObject;
        return SearchJsonValue(jValue,path,result);
    }

    static bool SearchJsonValue(const QJsonValue& jValue,const QStringList& path,QJsonValue& result)
    {
        result = jValue;
        for(int i=0;i<path.length();++i)
        {
            result = result[path[i]];
            if(result.isNull() || result.isUndefined())
            {
                return false;
            }
        }
        return true;
    }


private:
    static void PrintJsonObject(QJsonObject& obj,const QString& key,int retract=0)
    {
        //输出使用的缩进
        QString tabs = "";
        for(int i=0;i<retract;++i) {tabs+="    ";}

        qDebug() << tabs << key << "isObject() 有 " << obj.size() <<  " 个子对象 ";
        for(const QString& key: obj.keys())
        {
            QJsonValue jvalue = obj[key];
            PrintJsonValue(jvalue,key,retract+1);
        }

    }

    static void PrintJsonArray(QJsonArray& arr,const QString& key,int retract=0)
    {
        //输出使用的缩进
        QString tabs = "";
        for(int i=0;i<retract;++i) {tabs+="    ";}

        qDebug() << tabs << key << "isArray() 有 " << arr.size() <<  " 个子对象 ";
        for(QJsonArray::iterator it=arr.begin();it!=arr.end();++it)
        {
            QJsonValue jvalue = *it;
            PrintJsonValue(jvalue,"",retract+1);
        }
    }

    static void PrintJsonValue(QJsonValue& jvalue,const QString& key,int retract=0)
    {
        //输出使用的缩进
        QString tabs = "";
        for(int i=0;i<retract;++i) {tabs+="    ";}

        if(jvalue.isArray())
        {
            QJsonArray childArray = jvalue.toArray();
            PrintJsonArray(childArray,key,retract);
        }
        else if(jvalue.isObject())
        {
            QJsonObject childObject = jvalue.toObject();
            PrintJsonObject(childObject,key,retract);
        }
        else if(jvalue.isBool())
        {
            qDebug() << tabs << key << " isBool() " << jvalue.toBool();
        }
        else if(jvalue.isDouble())
        {
            qDebug() << tabs << key << " isDouble() " << jvalue.toDouble();
        }
        else if(jvalue.isString())
        {
            qDebug() << tabs << key << " isString() " << jvalue.toString();
        }
        else if(jvalue.isUndefined())
        {
            qDebug() << tabs << " isUndefined() ";
        }
        else if(jvalue.isNull())
        {
            qDebug() << tabs << " isNull() ";
        }
    }

};

#endif // ZJSONTOOLS_HPP
