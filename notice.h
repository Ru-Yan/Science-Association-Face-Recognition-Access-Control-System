#ifndef NOTICE_H
#define NOTICE_H
#include <QSqlTableModel>
#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
namespace Ui {
class Notice;
}

class Notice : public QMainWindow
{
    Q_OBJECT
public:
    explicit Notice(QWidget *parent = nullptr);
    ~Notice();

private slots:
    void on_backbt_clicked();
    void on_deletebt_clicked();
    void on_sendbt_clicked();
    void finishedSlot(QNetworkReply*);

private:
    Ui::Notice *ui;
    QSqlTableModel *model;
    QNetworkAccessManager *m_NetManger;
    QNetworkReply* m_Reply;
};

#endif // NOTICE_H
