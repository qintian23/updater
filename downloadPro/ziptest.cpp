#include "ziptest.h"
#include "ui_ziptest.h"

ZipTest::ZipTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZipTest)
{
    ui->setupUi(this);
    compreDecompreFileThread = new CompreDecompreFileThread;
    compreDecompreFileMainThread = new QThread;

    compreDecompreFileThread->moveToThread(compreDecompreFileMainThread);
    compreDecompreFileMainThread->start();

    connect(this,&ZipTest::compress_signal,compreDecompreFileThread,&CompreDecompreFileThread::compression_file_run);
    connect(this,&ZipTest::decompress_signal,compreDecompreFileThread,&CompreDecompreFileThread::decompression_file_run);

    connect(compreDecompreFileThread,&CompreDecompreFileThread::compression_res_signal,this,&ZipTest::compress_res_slot);
    connect(compreDecompreFileThread,&CompreDecompreFileThread::decompression_res_signal,this,&ZipTest::decompress_res_slot);
    qDebug()<<QThread::currentThreadId();

}

ZipTest::~ZipTest()
{
    delete ui;
}

void ZipTest::on_btnZip_clicked()
{
    QString filePath = QApplication::applicationDirPath()+"/test.txt";
    QString zipPath = QApplication::applicationDirPath();

    qDebug() << filePath << zipPath;

    emit compress_signal(filePath,zipPath);
}

void ZipTest::on_btnUnZip_clicked()
{
    // qtdeclarative-everywhere-src-5.14.2.tar.xz
//    QString filePath = QApplication::applicationDirPath()+"/QZipReader.zip";
    QString filePath = QApplication::applicationDirPath()+"/qtdeclarative-everywhere-src-5.14.2.tar.xz";
    QString zipPath = QApplication::applicationDirPath()+"/";

    qDebug() << filePath << zipPath;

    emit decompress_signal(filePath,zipPath);
}

void ZipTest::compress_res_slot(CompreDecompreFileThread::ResultE res, quint8 progress, QString remarks)
{
    qDebug()<<"Com:"<<"  res:"<<res<<"  progress:"<<progress<<"  remarks:"<<remarks;
}

void ZipTest::decompress_res_slot(CompreDecompreFileThread::ResultE res, quint8 progress, QString remarks)
{
    qDebug()<<"deCom:"<<"  res:"<<res<<"  progress:"<<progress<<"  remarks:"<<remarks;
}

