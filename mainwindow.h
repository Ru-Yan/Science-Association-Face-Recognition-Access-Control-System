#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QMainWindow>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QThread>
#include "mythread.h"
using namespace cv;
using namespace std;
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void run();
    void run_time();
    void tips(QString message);
    void tips_manger(QString message);
private slots:    
    void on_refresh_clicked();
    void on_recface_btn_clicked();
    void on_addface_btn_clicked();
    void on_enter_btn_clicked();
private:
    void pass(QString pressword);
    void to_menu(QString pressword);
    QPixmap PixmapToRound(const QPixmap &src, int radius);    
private:
    Ui::MainWindow *ui;
    QString pressword;  
    QTimer theTimer,timer;
    QString time;
    cv::Mat srcImage;
    FaceRecog *FR;
};

#endif // MAINWINDOW_H
