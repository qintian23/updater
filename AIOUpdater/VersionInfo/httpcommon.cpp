#include "httpcommon.h"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QNetworkRequest>
#include <QCryptographicHash>
#include <Downloader/jsonManager.h>

HttpCommon* HttpCommon::gInstance = 0;

HttpCommon::HttpCommon(QObject *parent)
    : QObject(parent)
    , mNetAccessManager(new QNetworkAccessManager(this))
{
    jsonManager mana;
    QJsonObject root = mana.getObj("code.json");
    QString token=root.value("access-token").toString();
    QString xtoken = root.value("x-access-token").toString();

    loginCacheToken = token.toLatin1().prepend(QByteArray("Bearer "));
    loginRefreshToken = xtoken.toLatin1().prepend(QByteArray("Bearer "));
}

void HttpCommon::clearRp(QNetworkReply *rp)
{
    if(rp)
    {
        QString url = rp->request().url().toString();
        QString postMD5 = rp->property("postMD5").toString();
        QString postData = rp->property("data").toByteArray();

        if(postMD5.isEmpty())
        {
            //清理对应缓存
            mDownloadDataCache.remove(url);
            //解除正在处理状态
            mProcessingRq.remove(url);
        }
        else
        {
            //清理对应缓存
            mDownloadDataCache.remove(postMD5);
            //解除正在处理状态
            mProcessingRq.remove(postMD5);
        }
        mRedirectMap.remove(url);
        mRedirectMap.remove(postMD5);

        qDebug() << "delete cache, url:" << url << " postMOD5:" << postMD5;
        rp->deleteLater();
    }
}

void HttpCommon::onHttpGetRspProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(sender() == NULL)
    {
        return ;
    }

    QNetworkReply* rp = qobject_cast<QNetworkReply*>(sender());
    if(rp == NULL)
    {
        return;
    }

    qDebug() << "http get rsp progress:" << rp->url().toString() << bytesReceived << "/" << bytesTotal;
    if(bytesTotal <= 0)
    {
        return;
    }

    QString url = rp->url().toString();

    mDownloadDataCache[url].append(rp->readAll());

}

void HttpCommon::onHttpGetRspFinished()
{
    if(sender() == NULL)
    {
        return ;
    }

    QNetworkReply* rp = qobject_cast<QNetworkReply*>(sender());
    if(rp == NULL)
    {
        return;
    }

    QByteArray rpData;
    QString url = rp->url().toString();

    int statusCode = rp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString strUrl = rp->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    switch(statusCode)
    {
    case 200: // ok
    {
        rpData = mDownloadDataCache[rp->url().toString()];

        QString redirectUrl = mRedirectMap[url];
        if(redirectUrl.isEmpty())
        {
            qDebug() << "http get rsp ok:" << url << "total size:" << rpData.count() << "bytes";
            emit httpGetRspReady(url, rpData);
        }
        else
        {
            qDebug() << "http get rsp ok:" << url << "total size:" << rpData.count() << "bytes" << "[redirect by]" << redirectUrl;
            emit httpGetRspReady(redirectUrl, rpData);
        }

    }
        break;
    case 301:
    case 302: // redirect
    {
        if(!strUrl.isEmpty())
        {
            QString turl = mRedirectMap[url];
            if(turl.isEmpty())
                mRedirectMap[strUrl] = url;
            else
                mRedirectMap[strUrl] = turl;

            httpGet(strUrl);
        }
    }
        break;
    default: // error
    {
        qDebug() << url << "[get error:" << statusCode << "]";
        QString redirectUrl = mRedirectMap[url];
        if(redirectUrl.isEmpty())
        {
            emit httpGetRspReady(url, QByteArray());
        }
        else
        {
            emit httpGetRspReady(redirectUrl, QByteArray());
        }
    }
        break;
    }

    clearRp(rp);
}

void HttpCommon::onHttpPostRspProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(sender() == NULL)
    {
        return ;
    }

    QNetworkReply* rp = qobject_cast<QNetworkReply*>(sender());
    if(rp == NULL)
    {
        return;
    }

    qDebug() << "http post rsp progress:" << rp->url().toString() << bytesReceived << "/" << bytesTotal;
    if(bytesTotal <= 0)
    {
        return;
    }

    QString postMD5 = rp->property("postMD5").toString();

    mDownloadDataCache[postMD5].append(rp->readAll());
}

void HttpCommon::onHttpPostRspFinished()
{
    if(sender() == NULL)
    {
        return ;
    }

    QNetworkReply* rp = qobject_cast<QNetworkReply*>(sender());
    if(rp == NULL)
    {
        return;
    }

    QByteArray rpData;

    QString url = rp->url().toString();
    QString postMD5 = rp->property("postMD5").toString();
    QByteArray postData = rp->property("data").toByteArray();

    int statusCode = rp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString strUrl = rp->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    if(loginCacheToken.length() < 13 || loginRefreshToken.length()<13)
    {
        loginCacheToken = rp->rawHeader("access-token").prepend(QByteArray("Bearer "));
        loginRefreshToken = rp->rawHeader("x-access-token").prepend(QByteArray("Bearer "));
        qDebug() << "======access-token=======" << loginCacheToken;
        qDebug() << "======x-access-token=======" << loginRefreshToken;
    }

    switch(statusCode)
    {
    case 200:
        {
            rpData = mDownloadDataCache[postMD5];

            QString redirectMD5 = mRedirectMap[postMD5];
            QString redirectUrl = mRedirectMap[url];
            if(redirectMD5.isEmpty() || redirectUrl.isEmpty())
            {
                qDebug() << "http post rsp ok:" << url << "total size:" << rpData.count() << "bytes";
                emit httpPostRspReady(url, postMD5, rpData);
            }
            else
            {
                qDebug() << "http post rsp ok:" << url << "total size:" << rpData.count() << "bytes" << "[redirect by]" << redirectUrl << "[data]" << postData;
                emit httpPostRspReady(redirectUrl, redirectMD5, rpData);
            }
        }
        break;
    case 301:
    case 302:
        {
            if(!strUrl.isEmpty())
            {
                QString turl = mRedirectMap[url];//direct by
                if(turl.isEmpty())
                    mRedirectMap[strUrl] = url;
                else
                    mRedirectMap[strUrl] = turl;


                QByteArray d;
                d.append(strUrl);
                d.append(postData);
                QString md5 = QCryptographicHash::hash(d, QCryptographicHash::Md5);
                QString tPostMD5 = mRedirectMap[md5];//direct by
                if(tPostMD5.isEmpty())
                {
                    mRedirectMap[md5] = postMD5;
                }
                else
                {
                    mRedirectMap[md5] = tPostMD5;
                }


                httpPost(strUrl, postData);
            }
        }
        break;
    default:
        qDebug() << url << "[post error:" << statusCode << "]";
        QString redirectMD5 = mRedirectMap[postMD5];
        QString redirectUrl = mRedirectMap[url];
        if(redirectMD5.isEmpty() || redirectUrl.isEmpty())
        {
            emit httpPostRspReady(url, postMD5, QByteArray());
        }
        else
        {
            emit httpPostRspReady(redirectUrl, redirectMD5, QByteArray());
        }
        break;
    }

    clearRp(rp);
}

HttpCommon *HttpCommon::instance()
{
    if(!gInstance)
        gInstance = new HttpCommon;

    return gInstance;
}

bool HttpCommon::hasInstance()
{
    return gInstance != NULL;
}

void HttpCommon::releaseInstance()
{
    if(gInstance)
        delete gInstance;

    gInstance = NULL;
}

void HttpCommon::httpGet(const QString &url)
{
    if(mProcessingRq.value(url, false)) //ignore when rq processing
    {
        return;
    }

    for(int i = 0; i < mRedirectMap.values().count(); i++)
    {
        if(mRedirectMap.values()[i] == url) //ignore when redirect processing
        {
            return;
        }
    }

    mProcessingRq.insert(url, true);

    QNetworkRequest rq;
    QSslConfiguration config = rq.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    rq.setSslConfiguration(config);

    rq.setRawHeader("Authorization", loginCacheToken);
    rq.setRawHeader("X-Authorization", loginRefreshToken);

    rq.setUrl(QUrl(url));
    QNetworkReply* rp = mNetAccessManager->get(rq);
    connect(rp, &QNetworkReply::finished        , this, &HttpCommon::onHttpGetRspFinished);
    connect(rp, &QNetworkReply::downloadProgress, this, &HttpCommon::onHttpGetRspProgress);
}

QByteArray HttpCommon::httpPost(const QString &url, const QByteArray &data, int type)
{
    QByteArray d;
    d.append(url);
    d.append(data);
    QByteArray md5 = QCryptographicHash::hash(d, QCryptographicHash::Md5);

    if(mProcessingRq.value(md5, false)) //ignore when rq processing
    {
        return md5;
    }

    for(int i = 0; i < mRedirectMap.values().count(); i++)
    {
        if(mRedirectMap.values()[i] == md5) //ignore when redirect processing
        {
            return md5;
        }
    }

    mProcessingRq.insert(md5, true);

    QNetworkRequest rq;
    QSslConfiguration config = rq.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    rq.setSslConfiguration(config);

    rq.setRawHeader("Authorization", loginCacheToken);
    rq.setRawHeader("X-Authorization", loginRefreshToken);

    rq.setUrl(QUrl(url));
    if(type != 0)
    {
        rq.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    }
    QNetworkReply* rp = mNetAccessManager->post(rq, data);
    rp->setProperty("postMD5", md5);
    rp->setProperty("url", url);
    rp->setProperty("data", data);

    connect(rp, &QNetworkReply::finished        , this, &HttpCommon::onHttpPostRspFinished);
    connect(rp, &QNetworkReply::downloadProgress, this, &HttpCommon::onHttpPostRspProgress);

    return md5;
}

