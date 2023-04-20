#include "updatedialog.h"
#include "ui_updatedialog.h"
#include <QGraphicsDropShadowEffect>


UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    this->setStyleSheet("border-radius:15px");

    ui->frame->setStyleSheet("background-color: rgb(0,0,0)");
    ui->frame->setStyleSheet("border-radius:8px");

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);

    shadow_effect->setColor(QColor(38, 78, 119, 127));
    shadow_effect->setBlurRadius(22);
    ui->frame->setGraphicsEffect(shadow_effect);
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}
