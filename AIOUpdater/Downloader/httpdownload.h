#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QFile>
#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QEventLoop>
#include <QCoreApplication>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

enum Status
{
    requestHead = 0,
    requestBody
};

class HttpDownLoad : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownLoad(QObject *parent = nullptr);
    ~HttpDownLoad();

public:
    void startDownLoad(const QString &url);
    void stopDownLoad();

private:
    void getCurrentFileSize();

signals:
    void fileDownloadProgress(qint64 cur, qint64 total);

private slots:
    void onfinishedRequest();
    void onReadyRead();
    void onError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;

    int m_fileSize;
    int m_statusCode;
    qint64 m_downLoadedBytes;
    qint64 m_currentLoadedBytes;

    QString m_fileName;
    QString m_url;
    Status m_state;

    bool m_IsDownloading;
    QFile m_file;
};

#endif // HTTPDOWNLOAD_H
