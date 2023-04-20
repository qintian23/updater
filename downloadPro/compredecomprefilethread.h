#ifndef COMPREDECOMPREFILETHREAD_H
#define COMPREDECOMPREFILETHREAD_H

#include <QDir>
#include <QFile>
#include <QThread>
#include <QObject>
#include <QFileInfoList>
#include <QtGui/private/qzipwriter_p.h>
#include <QtGui/private/qzipreader_p.h>

class CompreDecompreFileThread : public QObject
{
    Q_OBJECT
public:
    explicit CompreDecompreFileThread(QObject *parent = nullptr);

public:
    typedef  enum
    {
        success,
        failed,
        running
    }ResultE;
signals:
    void compression_res_signal(ResultE res,quint8 progress,QString remarks = nullptr);
    void decompression_res_signal(ResultE res,quint8 progress,QString remarks = nullptr);
public slots:
    /*
     * 压缩文件
     */
    void compression_file_run(QString srcPath,QString destPath);
    /*
     * 解压文件
     */
    void decompression_file_run(QString srcPath,QString destPath);
private:
    /*
     * 遍历压缩文件
     */
    QFileInfoList ergodic_compression_file(QZipWriter *writer,QString rootPath,QString filePath);
private:
    QZipWriter *zipWriter = nullptr;
    QZipReader *zipReader = nullptr;
};

#endif // COMPREDECOMPREFILETHREAD_H
