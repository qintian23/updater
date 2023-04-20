#ifndef ROLLBACKMECHANISM_H
#define ROLLBACKMECHANISM_H

#include <QObject>
#include <QFileInfo>

class RollbackMechanism : QObject
{
    Q_OBJECT
public:
    RollbackMechanism();
    ~RollbackMechanism(){}

public:
    // delete file or dir
    static bool DeleteCate(QString strDirPath);
    // backup file  strDestPath: backup path; strSrcPath: file path.
    static void BackupCate(QString srcDestPath, QString strSrcPath);
    // get file size
    static float CateSize(QString strDirPath);
    // regulate file (if the file is less than fsizeStd, delete it)
    static void ArrangeCate(QString strDirPath, float fSizeStd);
    // get the number of file
    static int GetFileNum(QString strDirPath);
    // create file and open file
    static bool CreateFile(QString);
    // ask the existen of the file
    static bool IsFileExist(QString);
    // copy the file
    static bool CopyFile(QString, QString);
    // rename the file
    static bool RenameFile(QString, QString);
    // write the file
    static void WriteFile(QString, QString);
    // Read the file
    static void ReadFile(QString);
    // delete the file
    static bool DeleteFile(QString);
    // Get all file in the folder
    static QList<QFileInfo> GetAllFileInfo(QString);
};

#endif // ROLLBACKMECHANISM_H
