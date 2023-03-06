#ifndef ENTITYBASE_H
#define ENTITYBASE_H

#include <QString>

#define ATTRIBUTE_MEMBER_FUNC(argType, arg)\
    public:\
    void set_##arg(const argType& v){\
        arg = v;\
    }\
    argType get_##arg(){\
        return arg;\
    }\

class EntityBase
{
protected:
    QString createTime;
    QString updateTime;
    QString createdUserId;
    QString createdUesrName;
    QString updatedUserId;
    QString updatedUserName;
    bool    isDelete;

public:
    /// <summary>
    /// 创建时间
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(QString, createTime);
    /// <summary>
    /// 更新时间
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(QString, updateTime);
    /// <summary>
    /// 创建者Id
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(QString, createdUserId);
    /// <summary>
    /// 创建者名称
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(QString, createdUesrName);
    /// <summary>
    /// 修改者Id
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(QString, updatedUserId);
    /// <summary>
    /// 修改者名称
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(QString, updatedUserName);
    /// <summary>
    /// 软删除
    /// </summary>
    ATTRIBUTE_MEMBER_FUNC(bool, isDelete);
};

#endif // ENTITYBASE_H
