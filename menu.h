#ifndef MENU_H
#define MENU_H
#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <vector>
#include "maininterface.h"
#include "test.h"
#include "mythread2.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
using namespace cv;
using namespace std;
namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private:
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    bool can_add();
signals:
    void addface_begin(String id);
private slots:
     void run2();
     void on_openbt_clicked();
     void on_addbt_clicked();
     void on_backbt_clicked();
     void on_school_currentIndexChanged(int index);
     void on_find_student_clicked();
     void on_check_xuyuan_currentIndexChanged(const QString &arg1);
     void on_check_class_currentIndexChanged(const QString &arg1);
     void finishedSlot(QNetworkReply*);
public slots:
     void addover_tips(QString message);
private:
    Ui::Menu *ui;
    QTimer theTimer2;
    cv::Mat srcImage2;
    FaceAdd *FA;
    QNetworkAccessManager *m_NetManger;
    QNetworkReply* m_Reply;
};

#endif // MENU_H
