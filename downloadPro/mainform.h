#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "http.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainform; }
QT_END_NAMESPACE

class mainform : public QWidget
{
    Q_OBJECT

public:
    explicit mainform(QWidget *parent = nullptr);
    ~mainform();

private slots:
    void on_btn_down_clicked();
    void onDownloadProgress(qint64 cur, qint64 total);
private:
    Ui::mainform *ui;

    http m_http;
    qint64 m_bytes;
    bool m_status;
};
#endif // MAINFORM_H
