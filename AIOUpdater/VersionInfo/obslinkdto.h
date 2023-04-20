#ifndef OBSLINKDTO_H
#define OBSLINKDTO_H

#include "entitybase.h"
#include <QByteArray>

class obsLinkDto
{
public:
    obsLinkDto();

private:
    int code;
    bool success;
    QString message;
    QString extras;
    QString data;
    QString timestamp;
    QString errorcode;

public:
    ATTRIBUTE_MEMBER_FUNC(int, code);
    ATTRIBUTE_MEMBER_FUNC(bool, success);
    ATTRIBUTE_MEMBER_FUNC(QString, message);
    ATTRIBUTE_MEMBER_FUNC(QString, extras);
    ATTRIBUTE_MEMBER_FUNC(QString, data);
    ATTRIBUTE_MEMBER_FUNC(QString, timestamp);
    ATTRIBUTE_MEMBER_FUNC(QString, errorcode);

public:
    bool init(QByteArray);
    QByteArray toString();
};

#endif // OBSLINKDTO_H
