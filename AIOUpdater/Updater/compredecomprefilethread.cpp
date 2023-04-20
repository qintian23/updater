#include "compredecomprefilethread.h"
#include "tar.h"
#include <QProcess>
#include <QDebug>
#include <QStringList>

CompreDecompreFileThread::CompreDecompreFileThread(QObject *parent) : QObject(parent)
{

}

QFileInfoList CompreDecompreFileThread::ergodic_compression_file(QZipWriter *writer, QString rootPath, QString filePath)
{
    QDir dir(filePath);
    QDir fileDir(filePath);

    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);   //获取当前所有目录 QDir::Dirs 0x001 列出目录；


        for(int i = 0; i != folderList.size(); i++)         //自动递归添加各目录到上一级目录
        {
            QString namepath = folderList.at(i).absoluteFilePath();    //获取路径
            QFileInfo folderinfo= folderList.at(i);
            QString name=folderinfo.fileName();      //获取目录名


            QStringList list = folderinfo.absoluteFilePath().split(rootPath);
            //qDebug()<<"Dir:"<<folderinfo.absoluteFilePath().remove(list.at(0));
            writer->addDirectory(folderinfo.absoluteFilePath().remove(list.at(0)));

            QFileInfoList child_file_list = ergodic_compression_file(writer,rootPath,namepath);          //进行递归 递归这个文件夹
            fileList.append(child_file_list);
            //  file_list.append(name);

        }
        /*添加path路径文件*/

        fileDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);        //获取当前所有文件
        fileDir.setSorting(QDir::Name);
        QFileInfoList _fileList = fileDir.entryInfoList();
        for (int i = 0; i < _fileList.size(); ++i) //将当前目录中所有文件添加到treewidget中
        {
            QFileInfo fileInfo = _fileList.at(i);

            QFile file(fileInfo.absoluteFilePath());
            if(file.open(QFile::ReadOnly))
            {
                QStringList list = fileInfo.absoluteFilePath().split(rootPath);
                //qDebug()<<"File:"<<fileInfo.absoluteFilePath().remove(list.at(0));
                writer->addFile(fileInfo.absoluteFilePath().remove(list.at(0)),file.readAll());
                file.close();
            }

        }
        return fileList;
}

void CompreDecompreFileThread::compression_file_run(QString srcPath, QString destPath)
{
    qDebug()<<QThread::currentThreadId();
    if(srcPath == nullptr || destPath == nullptr)
    {
        emit compression_res_signal(ResultE::failed,100,tr("路径为空！"));
        return;
    }
    QFileInfo fileDistInfo(destPath);
    if(fileDistInfo.isFile())   //目的路径不能为文件名
    {
        emit compression_res_signal(ResultE::failed,100,tr("路径不能为文件！"));
        return;
    }


    QFileInfo fileInfo(srcPath);
    if(fileInfo.isFile())//压缩的是一个文件
    {
        QString saveFileName = destPath+"/"+fileInfo.baseName()+".zip";
        qDebug()<<"saveFileName:"<<saveFileName;
        this->zipWriter = new QZipWriter(saveFileName);
        QFile file(srcPath);
        if(!file.open(QFile::ReadOnly))
        {
            emit compression_res_signal(ResultE::failed,100,tr("文件打开失败！"));
            return;
        }
        emit compression_res_signal(ResultE::running,50,tr("正在压缩中..."));
        this->zipWriter->addFile(file.fileName(),file.readAll());
        this->zipWriter->close();
        delete this->zipWriter;
        this->zipWriter = nullptr;
        emit compression_res_signal(ResultE::running,100,tr("正在压缩中..."));
        emit compression_res_signal(ResultE::success,100,tr("压缩完成！"));
    }
    else    //压缩的是一个文件夹
    {
        QString folderStr = srcPath.mid(srcPath.lastIndexOf("/")).remove("/");
        if(folderStr == nullptr)
        {
            emit compression_res_signal(ResultE::failed,100,tr("压缩文件路径错误！"));
            return;
        }
        emit compression_res_signal(ResultE::running,50,tr("正在压缩中..."));
        QString saveFileName = destPath+"/"+folderStr+".zip";
        qDebug()<<"saveFileName:"<<saveFileName;
        this->zipWriter = new QZipWriter(saveFileName);
        this->zipWriter->addDirectory(folderStr);
        this->ergodic_compression_file(this->zipWriter,folderStr,srcPath);
        delete this->zipWriter;
        this->zipWriter = nullptr;
        emit compression_res_signal(ResultE::running,100,tr("正在压缩中..."));
        emit compression_res_signal(ResultE::success,100,tr("压缩完成！"));
    }

}

bool CreateFolder(const QString& folderPath)
{
    bool state = false;

    //创建文件夹
    QDir dir(folderPath);
    if (!dir.exists())
    {
        return dir.mkdir(folderPath);
    }else{
        state = true;
    }

    return state;
}

void CompreDecompreFileThread::decompression_file_run(QString srcPathAndName, QString destPath)
{
    qDebug()<<QThread::currentThreadId();
    if(srcPathAndName == nullptr || destPath == nullptr)
    {
        emit decompression_res_signal(ResultE::failed,100,tr("路径为空！"));
        return;
    }
    QFileInfo destInfo(destPath);
    if(destInfo.isFile()||!destInfo.exists())
    {
        emit decompression_res_signal(ResultE::failed,100,tr("目的路径异常！"));
        return;
    }

    QFileInfo fileInfo(srcPathAndName);
    if(!fileInfo.isFile() || fileInfo.suffix() !="zip")
    {
        emit decompression_res_signal(ResultE::failed,100,tr("压缩文件为空！"));
        return;
    }
    emit decompression_res_signal(ResultE::running,50,tr("正在解压缩..."));
    this->zipReader = new QZipReader(srcPathAndName);
    bool res = false; //this->zipReader->extractAll(destPath);
    qDebug() << "====fileInfoList().size()=====" << this->zipReader->fileInfoList().size();
    for(auto fileInfo : this->zipReader->fileInfoList())
    {
        if(fileInfo.isFile)
        {
            qDebug() << "====**fileInfo.filePath**====" << fileInfo.filePath;
            QFile file(destPath + "/" + fileInfo.filePath);
            if(file.open(QIODevice::WriteOnly))
            {
                QByteArray dt = fileInfo.filePath.toUtf8();
                QString strtemp = QString::fromLocal8Bit(dt);
                QByteArray array = this->zipReader->fileData(strtemp);
                file.write(array);
                file.close();
            }
            res = true;
        }
        else if(fileInfo.isDir)
        {
            qDebug() << "====**fileInfo.filePath**====" << fileInfo.filePath;
            CreateFolder(destPath + "/" + fileInfo.filePath);
        }
    }

    emit decompression_res_signal(ResultE::running,100,tr("正在解压缩..."));
    if(res) //解压缩成功
    {
        emit decompression_res_signal(ResultE::success,100,tr("解压缩完成！"));
    }
    else
    {
         emit decompression_res_signal(ResultE::failed,100,tr("解压缩失败！"));
    }

    this->zipReader->close();
    delete this->zipReader;
    this->zipReader = nullptr;

}

bool CompreDecompreFileThread::Unzip(QString filePath, QString zipPath)
{
    QString cmd = QString("unzip -o %1 -d %2").arg(filePath).arg(zipPath);
    QProcess p;
    p.start("sh", QStringList() << "-c" << cmd);
    p.waitForFinished();
    QString result = p.readAllStandardOutput();
    return !result.contains("End-of-central-directory");
}
