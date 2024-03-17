#include "mythread2.h"
#include <QDebug>
#include <QString>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>
#include "test.h"
using namespace cv;
cv::VideoCapture capture2;
FaceAdd::FaceAdd(QObject *parent) : QThread(parent)
{

}
void FaceAdd::setId(String id){
    face_id=id;
}
void FaceAdd::run()
{
    for(int i=0;i<11;i++){
        cv::Mat m3;
        capture2>>m3;
        add_face(face_id,m3);
    }
    QString message = "用户: "+QString(QString::fromLocal8Bit(face_id.c_str()))+"采集完成";
    emit add_over(message);

}
