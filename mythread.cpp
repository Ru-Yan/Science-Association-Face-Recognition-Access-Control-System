#include "mythread.h"
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
cv::VideoCapture capture;
string name_rec;
float conf_fake;
float conf_mask;
float conf_face;

FaceRecog::FaceRecog(QObject *parent) : QThread(parent)
{

}
void FaceRecog::run()
{
   while(1)
   {
    cv::Mat m3;
    capture>>m3;
    name_rec = "unknown";
    conf_fake = 0.f;
    conf_mask = 0.f;
    conf_face = 0.f;
    rec_face(m3,name_rec,conf_fake,conf_mask,conf_face);
    if(name_rec=="unknown"){
        QString message = "用户不存在，请添加";
        emit recface_ok(message);
        break;
    }
    else if(name_rec=="marked"){
        QString message = "管理员：1962810202 登录成功";
        emit recface_ok(message);
        break;
    }
    else if(name_rec=="faked"){
        QString message = "活体检测未通过";
        emit recface_ok(message);
        break;
    }
    else{
        //连接数据库
        QSqlDatabase database;
        if (QSqlDatabase::contains("qt_sql_default_connection"))
        {
            database = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            database = QSqlDatabase::addDatabase("QSQLITE");
            database.setDatabaseName("MyDataBase.db");
            database.setUserName("123456");
            database.setPassword("123456");
        }
        //打开数据库
        if (!database.open())
        {
                qDebug() << "Error: Failed to connect database..." << database.lastError();
        }
        else
        {
                qDebug() << "Succeed to connect database..." ;
        }
        //现实时间和密码
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm");
        QString qid = name_rec.c_str();
        QSqlQuery query;
        query.exec("SELECT * FROM student WHERE id=="+qid+";");
        if(!query.next()){
             qDebug("信息有误，请重新输入学号");
             database.close();
             QString message = "用户: "+qid+"不存在";
             emit recface_ok(message);
             break;
        }
        else{
            if(query.value(5)=="普通用户"){
                        //设置打卡时间
                        QString sql = QString( "UPDATE student SET time = '%1' WHERE id = '%2'" ).arg(str).arg(qid);
                        query.prepare(sql);
                        if( !query.exec() )
                           qDebug() << "打卡时间更新失败！"<<query.lastError();
                        else{
                           QString message = "用户: "+qid+"打卡成功";
                           emit recface_ok(message);
                           qDebug( "打卡时间更新成功" );  
                        }
                        database.close();
                        break;
            }
            else if(query.value(5)=="管理员"){
                        QString message = "管理员: "+qid+"登录成功";
                        emit recManger_ok(message);
                        break;
            }
            else{
                qDebug()<<query.value(5);
                database.close();
                break;
            }
       }
    }

   }
}
