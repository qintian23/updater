#include "http.h"

http::http(QObject *parent) : QObject(parent),
    m_networkManager(nullptr),
    m_reply(nullptr),
    m_fileSize(0),
    m_statusCode(0),
    m_downLoadedBytes(0),
    m_currentLoadedBytes(0),
    m_fileName(""),
    m_url(""),
    m_IsDownloading(false)
{
    m_networkManager = new QNetworkAccessManager();
}

http::~http()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }
}

void http::stopDownLoad()
{
    qDebug() << "Enter stopDownLoad";
    if(m_reply == nullptr) return;

    disconnect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    disconnect(m_reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(onError(QNetworkReply::NetworkError)));
    disconnect(m_reply,SIGNAL(finished()),this,SLOT(onfinishedRequest()));

    m_reply->abort();
    m_reply->deleteLater();

    m_file.close();
    getCurrentFileSize();

    m_IsDownloading = false;
    qDebug() << "Leave stopDownLoad";
}

void http::getCurrentFileSize()
{
    qDebug() << "Enter getCurrentFileSize";
    QString path = QCoreApplication::applicationDirPath();
//    auto str2=QLatin1String(“sds”)+QLatin1String(“sdd”)+QLatin1String(“sds1”)+QLatin1String(“sds2”);
    auto tmpStr = path +  QLatin1String("/") + m_fileName;
    QString filepath = tmpStr;
    qDebug() << "getCurrentFileSize::filepath =  " << filepath;
    QFileInfo fileInfo(filepath);
    if(fileInfo.exists())
    {
        m_currentLoadedBytes = fileInfo.size();
    }
    else
    {
        m_currentLoadedBytes = 0;
    }
    qDebug() << "Leave getCurrentFileSize";
}

void http::startDownLoad(const QString &url)
{
    qDebug() << "Enter startDownLoad";
    m_url = url;
    if(m_url.isEmpty()) return;

    if(!m_IsDownloading)
    {
        QNetworkRequest request;
        QUrl url = QUrl(m_url);
        request.setUrl(url);
        m_fileName = url.fileName();
        qDebug() << "fileName = " << m_fileName;
        m_reply = m_networkManager->head(request);
        m_state = requestHead;
        getCurrentFileSize();
        connect(m_reply, SIGNAL(finished()), this, SLOT(onfinishedRequest()));
    }
    qDebug() << "Leave startDownLoad";
}

void http::onfinishedRequest()
{
    qDebug() << "Enter onfinishedRequest";
    if(m_reply == nullptr) return;

    if(m_state == requestHead)
    {
        m_fileSize = m_reply->rawHeader("Content-Length").toInt();
        qDebug() << "m_fileSize = " << m_fileSize;
        if(m_fileSize == 0)
        {
            qDebug() << "failed header" << m_fileSize;
        }
        if(m_currentLoadedBytes == m_fileSize)
        {
            qDebug() << "the file is existed" << m_fileSize;
            return;
        }
        QNetworkRequest request;
        request.setUrl(m_url);

        QEventLoop *loop = new QEventLoop;
        connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));
        m_reply = m_networkManager->get(request);
        loop->exec();

        m_statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "statusCode = " << m_statusCode;
        m_state = requestBody;
    }

    QNetworkRequest request;
    if(m_statusCode == 200)
    {
        request.setUrl(m_url);
    }
    else if(m_statusCode == 302)
    {
        QUrl realUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        request.setUrl(realUrl);
    }
    else
    {
        return;
    }

    QString downLoadSize = QString::number(m_fileSize);
    QString selectSize = QString("bytes=%1-%2").arg(m_currentLoadedBytes).arg(downLoadSize);
    request.setRawHeader("Range", selectSize.toLatin1());
    m_reply = m_networkManager->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(onfinishedRequest()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));

    qDebug() << "Leave onfinishedRequest";
}

void http::onReadyRead()
{
    qDebug() << "Enter onReadyRead";
    if(m_reply == nullptr) return;

    QString path = QCoreApplication::applicationDirPath();
    QString filepath = tr("%1/%2").arg(path).arg(m_fileName);
    qDebug() << "onReadyRead::filepath =  " << filepath;
    if(!m_file.isOpen())
    {
        m_file.setFileName(filepath);
        m_file.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    m_file.write(m_reply->readAll());
    m_downLoadedBytes = m_file.size();
    emit fileDownloadProgress(m_downLoadedBytes, m_fileSize);
    if(m_file.size() == m_fileSize)
    {
        qDebug() << "download finished !";
        stopDownLoad();
    }
    qDebug() << "Leave onReadyRead";
}

void http::onError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);
    if(m_reply == nullptr) return;
    qDebug() << "error:" << m_reply->errorString();
}
