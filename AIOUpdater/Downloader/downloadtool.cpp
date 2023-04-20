#include "downloadtool.h"

DownloadTool::DownloadTool(QObject* parent)
    : QObject(parent)
{

}

DownloadTool::~DownloadTool() {}

void DownloadTool::deleteFileHandle()
{
    file.reset(new QFile());
}

void DownloadTool::startDownload(const QString& downloadUrl, const QString& savePath)
{
    m_downloadUrl = downloadUrl;
    m_savePath    = savePath;

    const QUrl newUrl = QUrl::fromUserInput(m_downloadUrl);

    if (!newUrl.isValid()) {
#ifdef DOWNLOAD_DEBUG
        qDebug() << QString("Invalid URL: %1: %2").arg(m_downloadUrl, newUrl.errorString());
#endif // DOWNLOAD_DEBUG
        Q_EMIT sigDownloadFailed(QString("Invalid URL: %1: %2").arg(m_downloadUrl, newUrl.errorString()));
        return;
    }

    QString fileName = newUrl.fileName();

    if (fileName.isEmpty()) fileName = defaultFileName;
    if (m_savePath.isEmpty()) { m_savePath = QApplication::applicationDirPath() + "/tmp"; }
    if (!QFileInfo(m_savePath).isDir()) {
        QDir dir;
        dir.mkpath(m_savePath);
    }

    fileName.prepend(m_savePath + '/');
    if (QFile::exists(fileName)) { QFile::remove(fileName); }
    file = openFileForWrite(fileName);
    if (!file) return;

    startRequest(newUrl);
}

void DownloadTool::cancelDownload()
{
    httpRequestAborted = true;
    reply->abort();
}

void DownloadTool::httpFinished()
{
    QFileInfo fi;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        file.reset();
    }

    if (httpRequestAborted) {
        return;
    }

    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
#ifdef DOWNLOAD_DEBUG
        qDebug() << QString("Download failed: %1.").arg(reply->errorString());
#endif // DOWNLOAD_DEBUG
        Q_EMIT sigDownloadFailed(reply->errorString());
        return;
    }

    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (!redirectionTarget.isNull()) {
        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
        file = openFileForWrite(fi.absoluteFilePath());
        if (!file) { return; }
        startRequest(redirectedUrl);
        return;
    }

    Q_EMIT sigDownloadFinished();

#ifdef DOWNLOAD_DEBUG
    qDebug() << QString(tr("Downloaded %1 bytes to %2 in %3")
        .arg(fi.size()).arg(fi.fileName(), QDir::toNativeSeparators(fi.absolutePath())));
    qDebug() << "Finished";
#endif // DOWNLOAD_DEBUG
}

void DownloadTool::httpReadyRead()
{
    if (file) file->write(reply->readAll());
}

void DownloadTool::networkReplyProgress(qint64 bytesRead, qint64 totalBytes)
{
    qreal progress = qreal(bytesRead) / qreal(totalBytes);
    Q_EMIT sigProgress(bytesRead, totalBytes, progress);

#ifdef DOWNLOAD_DEBUG
    qDebug() << QString::number(progress * 100, 'f', 2) << "%    "
        << bytesRead / (1024 * 1024) << "MB" << "/" << totalBytes / (1024 * 1024) << "MB";
#endif // DOWNLOAD_DEBUG
}

void DownloadTool::startRequest(const QUrl& requestedUrl)
{
    url = requestedUrl;
    httpRequestAborted = false;

    reply = qnam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &DownloadTool::httpFinished);
    connect(reply, &QIODevice::readyRead, this, &DownloadTool::httpReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &DownloadTool::networkReplyProgress);

#ifdef DOWNLOAD_DEBUG
    qDebug() << QString(tr("Downloading %1...").arg(url.toString()));
#endif // DOWNLOAD_DEBUG
}

std::unique_ptr<QFile> DownloadTool::openFileForWrite(const QString& fileName)
{
    std::unique_ptr<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
#ifdef DOWNLOAD_DEBUG
        qDebug() << QString("Unable to save the file %1: %2.")
            .arg(QDir::toNativeSeparators(fileName), file->errorString());
#endif // DOWNLOAD_DEBUG
        QString(QString("Unable to save the file %1: %2.")
                .arg(QDir::toNativeSeparators(fileName), file->errorString()));
        return nullptr;
    }
    return file;
}
