#ifndef UPDATEVERQUERYDTO_H
#define UPDATEVERQUERYDTO_H

#include <QString>

class UpdateVerQueryDto
{
public:
    UpdateVerQueryDto();
    virtual ~UpdateVerQueryDto(){}

private:
    void SetClientType(int);
    void SetVersion(QString);
    void SetIsEnabled(bool);

    void GetUpdateVersion();
    void SetUpdateInfo(QByteArray);

public:
    int ClientType;
    QString Version;
    bool IsEnabled;
};

#endif // UPDATEVERQUERYDTO_H
