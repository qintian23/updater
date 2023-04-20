#include "updaterdialog.h"
#include "ui_updaterdialog.h"
#include "../Logging/updatelog.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>

UpdaterDialog::UpdaterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdaterDialog)
{
    _url = " ";
    _path = " ";
    _reDownloadTimes = 0;
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    //外层窗口显示为透明
    setAttribute(Qt::WA_TranslucentBackground,true);
    //内层窗口添加对应的阴影效果
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(QColor(150,150,150));
    shadow_effect->setBlurRadius(6);
    ui->inner_widget->setGraphicsEffect(shadow_effect);
    //设置内层QWidget的边框和背景颜色
    ui->inner_widget->setStyleSheet("QWidget#inner_widget{border:1px solid #FFFFFF;border-radius:7px;background-color:#FFFFFF;}");

    m_status = true;
    qDebug() << "OpenSSL Support: " << QSslSocket::supportsSsl();
    dt = new DownloadTool();
    connect(dt, SIGNAL(sigProgress(qint64, qint64, qreal)), this, SLOT(onDownloadProgress(qint64, qint64, qreal)));
    connect(dt, SIGNAL(sigDownloadFailed(QString)), this, SLOT(onDownloadFailed(QString)));
//    connect(dt, SIGNAL(sigDownloadFinished()), this, SLOT(SendDownloadFinished()));
}

UpdaterDialog::~UpdaterDialog()
{
    delete dt;
    dt = nullptr;
    delete ui;
}

void UpdaterDialog::onDownloadProgress(qint64 cur, qint64 total, qreal progress)
{
    m_status = false;
    QString totalSize = QString::number(total / (1024 * 1024)) + "M";
    QString currentSize = QString::number(cur / (1024 * 1024)) + "M";
    ui->lbStatus->setText("下载中..."+currentSize +"/"+totalSize);
    qDebug() << "--" << progress;
    ui->pbUpdate->setMaximum(total);
    ui->pbUpdate->setValue(cur);

    if(cur == total)
    {
        ui->lbStatus->setText("系统更新中...");
        dt->deleteFileHandle();
        UpdateLog::StepDownload = true;
        Q_EMIT fileDownloadStatu(true);
    }
}

void UpdaterDialog::onDownload(QString url, QString path)
{
    _url = url;
    _path = path;
    if(m_status)
    {
        dt->startDownload(url, path);
//        m_http.startDownLoad(url);
//        qDebug() << "-------------------------";
        ui->lbStatus->setText("网络请求中...");
    }
}

void UpdaterDialog::onDownloadFailed(QString errorMsg)
{
    if(_reDownloadTimes++ < 3)
    {
        SetStatus(errorMsg);
        QEventLoop loop;
        QTimer::singleShot(3000, &loop, SLOT(quit()));
        loop.exec();
        onDownload(_url, _path);
    }
    else
    {
        SetStatus("更新文件下载失败");
        QEventLoop loop;
        QTimer::singleShot(3000, &loop, SLOT(quit()));
        loop.exec();
        UpdateLog::StepDownload = false;
        Q_EMIT fileDownloadStatu(false);
    }
}

void UpdaterDialog::SetStatus(QString status)
{
    ui->lbStatus->setText(status);
}
