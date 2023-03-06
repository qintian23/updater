#ifndef SYSFILE_H
#define SYSFILE_H

#include "EntityBase.h"
#include <QByteArray>

enum OSSProvider
{
    /// <summary>
    /// 无效
    /// </summary>
    Invalid = 0,
    /// <summary>
    /// Minio自建对象储存
    /// </summary>
    Minio = 1,
    /// <summary>
    /// 阿里云OSS
    /// </summary>
    Aliyun = 2,
    /// <summary>
    /// 腾讯云OSS
    /// </summary>
    QCloud = 3,
    /// <summary>
    /// 七牛云 OSS
    /// </summary>
    Qiniu = 4,
    /// <summary>
    /// 华为云 OBS
    /// </summary>
    HuaweiCloud = 5
};

class SysFile : public EntityBase
{
public:
    SysFile();

private:
    QString     id;
    OSSProvider fileLocation;
    QString     bucketName;
    QString     fileOriginName;
    QString     fileObjectName;
    QString     suffix;
    QString     filePath;
    QString     sizeKb;
    QString     sizeInfo;
    QString     md5;

public:
    ATTRIBUTE_MEMBER_FUNC(QString, id);
    ATTRIBUTE_MEMBER_FUNC(OSSProvider, fileLocation);
    ATTRIBUTE_MEMBER_FUNC(QString, bucketName);
    ATTRIBUTE_MEMBER_FUNC(QString, fileOriginName);
    ATTRIBUTE_MEMBER_FUNC(QString, fileObjectName);
    ATTRIBUTE_MEMBER_FUNC(QString, suffix);
    ATTRIBUTE_MEMBER_FUNC(QString, filePath);
    ATTRIBUTE_MEMBER_FUNC(QString, sizeKb);
    ATTRIBUTE_MEMBER_FUNC(QString, sizeInfo);
    ATTRIBUTE_MEMBER_FUNC(QString, md5);

public:
    void InitFileInfo(QString);

private:
    void setFileInfo(QByteArray);
};

#endif // SYSFILE_H
