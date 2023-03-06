#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget *parent = nullptr);
    ~UpdateDialog();

public:
    void SetRadius(int radius);
    void SetRadiusEnable(bool left_top, bool left_bottom, bool right_left, bool right_bottom);

protected:
    virtual void paintEvent(QPaintEvent* event)override;

private:
    int radius_ = {0};
    QColor bg_color_ = {106 ,90 ,205};
    bool left_top_=false;
    bool left_bottom_ = false;
    bool right_top_ = false;
    bool right_bottom_ = false;

private:
    Ui::UpdateDialog *ui;
};

#endif // UPDATEDIALOG_H
