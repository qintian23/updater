#ifndef UPDATEVERQUERYDTO_H
#define UPDATEVERQUERYDTO_H

#include "entitybase.h"
#include <QJsonObject>
#include <QString>

class UpdateVerQueryDto
{
public:
    UpdateVerQueryDto();
    virtual ~UpdateVerQueryDto(){}

private:
    QString id;
    int     clientType;
    QString version;
    bool    isEnabled;
    bool    isDeleted;
    QString updateContent;
    QString updateFileId;
    QString remark;
    QString operators;
    QString operateTime;
    QString creator;
    QString createTime;

public:
    ATTRIBUTE_MEMBER_FUNC(QString, id);
    ATTRIBUTE_MEMBER_FUNC(int, clientType);
    ATTRIBUTE_MEMBER_FUNC(QString, version);
    ATTRIBUTE_MEMBER_FUNC(bool, isEnabled);
    ATTRIBUTE_MEMBER_FUNC(bool, isDeleted);
    ATTRIBUTE_MEMBER_FUNC(QString, updateContent);
    ATTRIBUTE_MEMBER_FUNC(QString, updateFileId);
    ATTRIBUTE_MEMBER_FUNC(QString, remark);
    ATTRIBUTE_MEMBER_FUNC(QString, operators);
    ATTRIBUTE_MEMBER_FUNC(QString, operateTime);
    ATTRIBUTE_MEMBER_FUNC(QString, creator);
    ATTRIBUTE_MEMBER_FUNC(QString, createTime);

public:
    bool init(QJsonObject);
    QByteArray toString();

};

#endif // UPDATEVERQUERYDTO_H
