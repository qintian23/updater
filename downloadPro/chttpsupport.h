#ifndef CHTTPSUPPORT_H
#define CHTTPSUPPORT_H

#include <QObject>
#include <QNetworkAccessManager>

class chttpsupport : public QObject
{
    Q_OBJECT
public:
    explicit chttpsupport(QObject *parent = nullptr);
    virtual ~chttpsupport(){}

private:
    static chttpsupport*        gInstance;

    QNetworkAccessManager*      mNetAccessManager;
    QString                     mCacheRoot;                   // 缓存目录
    QMap<QString, bool>         mProcessingRq;                // 当前正在处理的请求 url 和是否正在处理
    QMap<QString, QByteArray>   mDownloadDataCache;           // 数据缓存url -> data | postMD5 -> data
    QMap<QString, QString>      mRedirectMap;                 // 重定向关系

signals:
    ///
    /// \brief httpGetRspReady http get 请求得到回复
    /// \param url             请求的地址
    /// \param data            回复的数据（如果isEmpty则表示请求出错了）
    ///
    void httpGetRspReady(QString url, QByteArray data);
    ///
    /// \brief httpPostRspReady http post 请求得到回复
    /// \param url              请求的地址
    /// \param postMD5          是post的时候url+数据的md5
    /// \param data             data是回复的数据（如果isEmpty则表示请求出错了）
    ///
    void httpPostRspReady(QString url, QString postMD5, QByteArray data);

private slots:
    void clearRp(QNetworkReply* rp);

    void onHttpGetRspProgress(qint64 bytesReceived, qint64 bytesTotal); //* http get 回复进度
    void onHttpGetRspFinished(); //* http get 处理完毕

    void onHttpPostRspProgress(qint64 bytesReceived, qint64 bytesTotal); //* http post 回复进度
    void onHttpPostRspFinished(); //* http get 处理完毕

public:
    static chttpsupport*    instance        ();
    static bool             hasInstance     ();
    static void             releaseInstance ();

    /////
    /// \brief httpGet 发起一个http get 请求
    /// \param url     请求地址
    ///
    void httpGet(const QString& url);

    ///
    /// \brief httpPost 发起一个http post 请求
    /// \param url      是请求的链接
    /// \param data     是传送的数据
    /// \return         返回url+data的md5
    ///
    QByteArray httpPost(const QString& url, const QByteArray &data);
};

#endif // CHTTPSUPPORT_H
