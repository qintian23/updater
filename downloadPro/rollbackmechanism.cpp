#include "rollbackmechanism.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QFileDialog>
#include <QMessageBox>

RollbackMechanism::RollbackMechanism() : QObject()
{

}

// delete folder
bool RollbackMechanism::DeleteCate(QString strDirPath)
{
    QDir dir(strDirPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden |QDir::NoDotAndDotDot);
    // delete file in the folder
    for(int i=0; i<fileList.size(); ++i)
    {
        QFileInfo fileInfo = fileList.at(i);
        if(!dir.remove(fileInfo.absoluteFilePath()))
        {
            qDebug() << tr("Deleting [%1] fail!").arg(fileInfo.completeBaseName());
            return false;
        }
    }
    QFileInfoList dirList = dir.entryInfoList(QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot);
    //递归调用删除文件夹内的文件
    for (int i=0;i<dirList.size();++i)
    {
        DeleteCate(dirList.at(i).absoluteFilePath());
    }

//    //删除该文件夹
//    if (!dir.rmdir(strDirPath))
//    {
//        QMessageBox::information(NULL,tr("Information"),tr("Deleting [%1] fail!").arg(strDirPath.section('/',-1)));
//        return false;
//    }
    return true;
}

// backup folder  strDestPath: backup path; strSrcPath: file path.
void RollbackMechanism::BackupCate(QString strDestPath, QString strSrcPath)
{
    QDir dir;
    if (!dir.exists(strDestPath))
        dir.mkpath(strDestPath);
    dir.setPath(strSrcPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
    //复制文件夹内的文件
    for (int i=0;i<fileList.size();++i)
    {
        QFileInfo fileInfo = fileList.at(i);
        if (!QFile::copy(fileInfo.absoluteFilePath(),strDestPath+"\\"+fileInfo.completeBaseName()+"."+fileInfo.completeSuffix()))
        {
            QMessageBox::information(NULL,tr("Information"),tr("Copying [%1] fail!").arg(fileInfo.completeBaseName()));
            return;
        }
    }
    QFileInfoList dirList = dir.entryInfoList(QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot);
    for (int i=0;i<dirList.size();++i)
    {
        QString strDeepDestPath = strDestPath + "/" + dirList.at(i).completeBaseName();
        QString strDeepSrcPath = strSrcPath + "/" + dirList.at(i).completeBaseName();
        BackupCate(strDeepDestPath,strDeepSrcPath);
    }
}

// get file size
float RollbackMechanism::CateSize(QString strDirPath)
{
    float fSize = 0;
    QDir dir(strDirPath);
    foreach(QFileInfo fileInfo,dir.entryInfoList(QDir::Files))
        fSize+=fileInfo.size();
    foreach(QString subDir,dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
        fSize+=CateSize(strDirPath+QDir::separator()+subDir)*1024*1024*1024;
    //fSize = fSize/(1024*1024*1024);
    fSize = fSize/(1024);
    return fSize;
}

// regulate file (if the file is less than fsizeStd, delete it)
void RollbackMechanism::ArrangeCate(QString strDirPath, float fSizeStd)
{
    float fSize = CateSize(strDirPath);
    if (fSize>fSizeStd)
    {
        QDir dir(strDirPath);
        dir.setSorting(QDir::Time|QDir::Reversed );
        QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        //QString str;
        //foreach(QFileInfo fileInfo,fileList)
        //	str = fileInfo.completeBaseName();
        if(!DeleteCate(strDirPath+QDir::separator()+ fileList.at(0).completeBaseName()))
            return;
        ArrangeCate(strDirPath,fSizeStd);
    }
}

// get the number of file
int RollbackMechanism::GetFileNum(QString strDirPath)
{
    int iSize = 0;
    QDir dir(strDirPath);
    foreach(QFileInfo fileInfo,dir.entryInfoList(QDir::Files))
        iSize++;
    foreach(QString subDir,dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
        iSize+=GetFileNum(strDirPath+QDir::separator()+subDir);
    return iSize;
}


// create file and open file
bool RollbackMechanism::CreateFile(QString filePath)
{
    QFile file(filePath);
    bool ok = file.open(QIODevice::ReadWrite);
    file.close();
    return  ok;
}

// ask the existen of the file
bool RollbackMechanism::IsFileExist(QString filePath)
{
    bool exist = QFile::exists(filePath);
    return  exist;
}

// copy the file
bool RollbackMechanism::CopyFile(QString curFilePath, QString newFilePath)
{
    bool ok = QFile::copy(curFilePath, newFilePath);
    return ok;
}

// rename the file
bool RollbackMechanism::RenameFile(QString curName, QString newName)
{
    QFile file(curName);
    bool ok = file.rename(newName);
    if(ok)
    {
        QFile::remove(curName);
    }
    file.close();
    return ok;

}

// write the file
void RollbackMechanism::WriteFile(QString filePath, QString writeInfo)
{
    QFile file(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.write(writeInfo.toUtf8());
    file.close();
}

// Read the file
void RollbackMechanism::ReadFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        qDebug() << line;
    }
}
// delete the file
bool RollbackMechanism::DeleteFile(QString filePath)
{
//    QFile file(filePath);
//    bool ok = file.remove();
    bool ok = QFile::remove(filePath);
    return ok;
}

// Get all file in the folder
QList<QFileInfo> RollbackMechanism::GetAllFileInfo(QString folderPath)
{
    QDir *dir = new QDir(folderPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QList<QFileInfo> *fileListInfo = new QList<QFileInfo>(dir->entryInfoList(filters));
    for(int i=0; i< fileListInfo->count(); i++)
    {
        qDebug() << fileListInfo->at(i).fileName();
    }
    return *fileListInfo;
}
