#include "foldercontroller.h"
#include "ui_foldercontroller.h"
#include "rollbackmechanism.h"

FolderController::FolderController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FolderController)
{
    ui->setupUi(this);
    ui->pathlineEdit->setText("D:\\liuqs\\Qt\\log\\");
}

FolderController::~FolderController()
{
    delete ui;
}

void FolderController::on_delect_btn_clicked()
{
    QString FIlePath = ui->pathlineEdit->text();
    RollbackMechanism::DeleteCate(FIlePath);
}

void FolderController::on_backup_btn_clicked()
{
    QString FilePath = ui->pathlineEdit->text();
    RollbackMechanism::BackupCate(FilePath+"/../temp/", FilePath);
}

void FolderController::on_getSize_btn_clicked()
{
    QString FilePath = ui->pathlineEdit->text();
    //float size = CMyDir::CateSize(FilePath);
    float size = RollbackMechanism::CateSize(FilePath);
    ui->lineEdit_2->setText(QString::number(size) + " K");

}

void FolderController::on_getCount_btn_clicked()
{
    QString FilePath = ui->pathlineEdit->text();
    int count = RollbackMechanism::GetFileNum(FilePath);
    ui->lineEdit_2->setText(QString::number(count));

}

void FolderController::on_arrange_btn_clicked()
{
    QString FilePath = ui->pathlineEdit->text();
        //CMyDir::ArrangeCate(FilePath);
}
