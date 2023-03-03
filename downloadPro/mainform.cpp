#include "mainform.h"
#include "ui_mainform.h"
#include "upgradeaccess.h"
//#include "httpcomm.h"
#include "chttpsupport.h"
#include "updateverquerydto.h"

mainform::mainform(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainform)
{
    ui->setupUi(this);
    m_status = true;
    qDebug() << "OpenSSL Support: " << QSslSocket::supportsSsl();

    connect(&m_http, SIGNAL(fileDownloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
    ui->edit_url->setPlaceholderText("请输入下载地址");
    ui->edit_url->setText("https://mirrors.ustc.edu.cn/qtproject/archive/qt/5.14/5.14.2/submodules/qtdeclarative-everywhere-src-5.14.2.tar.xz");
    ui->edit_url->setToolTip(ui->edit_url->text());
}

mainform::~mainform()
{
    delete ui;
}

QByteArray val;

void test_2(QByteArray d)
{
    qDebug() << d;
//    val = d;
}

void test_3()
{
    qDebug() << val;
    UpdateVerQueryDto dto = UpdateVerQueryDto();
    qDebug() << dto.Version;
}

void test()
{
//    httpcomm tt = httpcomm("http://120.77.219.29:8080/api/internal/SysUpdateVerMgr/VerList");
//    tt.GetRequest();

//    QByteArray val;

    QObject::connect(chttpsupport::instance(), &chttpsupport::httpGetRspReady, [=](QString url, QByteArray d){
            if(url!=nullptr)
            {
                qDebug() << "GET";
//                qDebug() << d;
//                test_2(d);
                val = d; //
                test_3();
            }
        });
    chttpsupport::instance()->httpGet("http://120.77.219.29:8080/");


//    upgradeaccess a = upgradeaccess();
//    QString tmpUrl = a.GetUrl("/Hello", true);
//    qDebug() << tmpUrl;

//    qDebug() << val;
}



void test_1()
{
    QObject::connect(chttpsupport::instance(), &chttpsupport::httpGetRspReady, [](QString url, QByteArray d){
            if(url == "http://vip.5211game.com/")
            {
                qDebug() << "GET";
                qDebug() << d;
            }
        });

    chttpsupport::instance()->httpGet("http://vip.5211game.com/");
}

void mainform::on_btn_down_clicked()
{
    test();
//    if(m_status)
//    {
//        m_http.startDownLoad(ui->edit_url->text());
//        ui->label->setText("网络请求中...");
//    }
//    else
//    {
//        m_http.stopDownLoad();
//        ui->btn_down->setText("下载");
//        ui->label->setText("已暂停");
//        m_status = true;
//    }
}

void mainform::onDownloadProgress(qint64 cur, qint64 total)
{
    ui->btn_down->setText("暂停");
    m_status = false;

    QString totalSize = QString::number(total/(1024*1024)) + "M";
    QString currentSize = QString::number(cur/(1024*1024)) + "M";
    ui->label->setText("下载中..."+currentSize +"/"+totalSize);

    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(cur);

    if(cur == total)
    {
        ui->label->setText("下载完毕");
        ui->btn_down->setText("下载");
    }
}
