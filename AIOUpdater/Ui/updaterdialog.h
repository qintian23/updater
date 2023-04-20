#ifndef UPDATERDIALOG_H
#define UPDATERDIALOG_H

//#include "Downloader/httpdownload.h"
#include "Downloader/downloadtool.h"
#include <QDialog>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class UpdaterDialog;
}

class UpdaterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdaterDialog(QWidget *parent = nullptr);
    ~UpdaterDialog();

signals:
    void fileDownloadStatu(bool);

private slots:
    void onDownloadProgress(qint64, qint64, qreal);
    void onDownloadFailed(QString);
private:
    Ui::UpdaterDialog *ui;
//    HttpDownLoad m_http;
    DownloadTool *dt;
    qint64 m_bytes;
    bool m_status;

    QString _url;
    QString _path;
    int _reDownloadTimes;

//private:
//    void SendDownloadFinished();

public:
    void onDownload(QString, QString);
    void SetStatus(QString);
};

#endif // UPDATERDIALOG_H
