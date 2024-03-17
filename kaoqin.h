#ifndef KAOQIN_H
#define KAOQIN_H
#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class Kaoqin;
}

class Kaoqin : public QWidget
{
    Q_OBJECT
public:
    explicit Kaoqin(QWidget *parent = 0);
    ~Kaoqin();
private slots:
    void on_backbt_clicked();
    void on_refresh_clicked();
    void finishedSlot(QNetworkReply*);
private:
    Ui::Kaoqin *ui;
    QNetworkAccessManager *m_NetManger;
    QNetworkReply* m_Reply;
    int count;
};

#endif // KAOQIN_H
