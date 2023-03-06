#ifndef ZIPTEST_H
#define ZIPTEST_H

#include <QDebug>
#include <QMainWindow>
#include "compredecomprefilethread.h"

namespace Ui {
class ZipTest;
}

class ZipTest : public QMainWindow
{
    Q_OBJECT
signals:
    void compress_signal(QString srcPath,QString destPath);
    void decompress_signal(QString srcPath,QString destPath);
public:
    explicit ZipTest(QWidget *parent = nullptr);
    ~ZipTest();

private slots:
    void on_btnZip_clicked();

    void on_btnUnZip_clicked();

    void compress_res_slot(CompreDecompreFileThread::ResultE res,quint8 progress,QString remarks = nullptr);
    void decompress_res_slot(CompreDecompreFileThread::ResultE res,quint8 progress,QString remarks = nullptr);


private:
    Ui::ZipTest *ui;
    CompreDecompreFileThread *compreDecompreFileThread;
    QThread *compreDecompreFileMainThread;
};

#endif // ZIPTEST_H
