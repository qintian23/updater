#ifndef FOLDERCONTROLLER_H
#define FOLDERCONTROLLER_H

#include <QMainWindow>

namespace Ui {
class FolderController;
}

class FolderController : public QMainWindow
{
    Q_OBJECT

public:
    explicit FolderController(QWidget *parent = nullptr);
    ~FolderController();

private slots:
    void on_delect_btn_clicked();

    void on_backup_btn_clicked();

    void on_getSize_btn_clicked();

    void on_getCount_btn_clicked();

    void on_arrange_btn_clicked();

private:
    Ui::FolderController *ui;
};

#endif // FOLDERCONTROLLER_H
