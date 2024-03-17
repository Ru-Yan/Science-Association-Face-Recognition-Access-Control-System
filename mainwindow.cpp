#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "menu.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <maininterface.h>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>
#include "net.h"
#include <vector>
#include "test.h"
#include <thread>
#include <QInputDialog>

using namespace std;
using namespace cv;
extern cv::VideoCapture capture;
extern string name_rec;
extern float conf_fake;
extern float conf_mask;
extern float conf_face;
QString mouth[12] = {"01","02","03","04","05","06","07","08","09","10","11","12"};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); 
    FR = new FaceRecog;
    connect(&theTimer, SIGNAL(timeout()), this, SLOT(run()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(run_time()));
    connect(FR,SIGNAL(recface_ok(QString)),this,SLOT(tips(QString)));
    connect(FR,SIGNAL(recManger_ok(QString)),this,SLOT(tips_manger(QString)));
    timer.start(1000);
    time = QDateTime::currentDateTime().toString("MM-dd");

    ui->label_wait->setText(" ");
    ui->verticalFrame->hide();
}
MainWindow::~MainWindow()
{
    if(capture.isOpened())
        capture.release();
    delete ui;
}
void MainWindow::tips(QString message){
    FR->quit();
    FR->wait();
    FR->deleteLater();
    theTimer.stop();
    capture.release();
    QMessageBox::about(this,"提示",message);
    MainWindow *w = new MainWindow();
    w->showFullScreen();
    this->close();

}

void MainWindow::tips_manger(QString message){

    FR->quit();
    FR->wait();
    FR->deleteLater();
    theTimer.stop();
    capture.release();
    QMessageBox::about(this,"提示",message);
    MainInterface *w = new MainInterface();
    w->showFullScreen();
    this->close();
}
//验证输入密码是否正确
void MainWindow::pass(QString pressword)
{
    if (capture.isOpened())
        return;
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
        QString qid = pressword;
        QSqlQuery query;
        query.exec("SELECT * FROM student WHERE id=="+qid+";");
        if(!query.next()){
             qDebug("信息有误，请重新输入学号");
             database.close();
             QString message = "用户: "+pressword+"不存在，请重新输入";
             QMessageBox::about(this,"提示",message);
        }
        else{
            if(query.value(5)=="普通用户"){

              QString message = "用户不具有管理员权限";
              QMessageBox::about(this,"提示",message);
              database.close();
            }
            else if(query.value(5)=="管理员"){
               QString message = "管理员: "+pressword+"登录成功";
               QMessageBox::about(this,"提示",message);
               this->close();
               MainInterface *w = new MainInterface();
               //将管理员名字传送过去
               w->setAdmin(pressword);
               w->showFullScreen();
            }
            else{
                qDebug()<<query.value(5);
                database.close();
            }

       }
}
//人脸识别
void MainWindow::run()
{
    //获取模型
    capture>>srcImage;
    cvtColor(srcImage,srcImage,CV_BGR2RGB);
    QImage image = QImage((uchar*)(srcImage.data), srcImage.cols, srcImage.rows, srcImage.step1(), QImage::Format_RGB888);
    QPixmap mmp=QPixmap::fromImage(image);
    mmp = mmp.scaled(ui->label_cricle_big->size());
    mmp =  PixmapToRound(mmp, 110);
    ui->label_top_2->setPixmap(mmp);
    ui->label_cricle->hide();

    //-----------------------------
    ui->name_tip->setText(QString::fromStdString(name_rec));

    std::ostringstream oss1;
    oss1<<conf_fake;
    std::string str1(oss1.str());
    QString fake=QString::fromStdString(str1);
    if(fake=="0"){
        ui->conf_fake_tip->setText("正在加载...");
    }else{
        ui->conf_fake_tip->setText(fake);
    }

    std::ostringstream oss2;
    oss2<<conf_mask;
    std::string str2(oss2.str());
    QString mask=QString::fromStdString(str2);
    if(mask=="0"){
        ui->conf_mask_tip->setText("正在加载....");
    }else{
        ui->conf_mask_tip->setText(mask);
    }

    std::ostringstream oss3;
    oss3<<conf_face;
    std::string str3(oss3.str());
    QString face=QString::fromStdString(str3);
    if(face=="0"){
        ui->conf_face_tip->setText("正在加载....");
    }else{
        ui->conf_face_tip->setText(face);
    }
    //---------------------------

}
//将图片切成圆形
QPixmap MainWindow::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(10, 0, size.width(), size.height(), 300, 300);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void MainWindow::run_time()
{
    //获取当前时间
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    ui->label_month->setText(time);
    QString time1 = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->label_time->setText(time1);
}

void MainWindow::on_refresh_clicked()
{
    MainWindow *w = new MainWindow();
    w->showFullScreen();
    this->close();
}

void MainWindow::on_recface_btn_clicked()
{
    if (capture.open(200)) {  
        cout<<"cam is opened"<<endl;
        //实例化人脸识别线程
        FR->start();
        ui->label_wait->setText("正在验证.....请等待");
        ui->verticalFrame->show();
        //从摄像头捕获视频
        theTimer.start(33);
    }else {
        cout<<"cam is not opened"<<endl;
        theTimer.stop();
        capture.release();
    }
}

void MainWindow::on_addface_btn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("进入系统添加人脸"),tr("请输入管理员号码(默认号码：12345)"), QLineEdit::Password,"12345", &ok);
    if (ok && !text.isEmpty()){
        qDebug()<<text;
        to_menu(text);
    }

}
void MainWindow::to_menu(QString pressword)
{
    if (capture.isOpened())
        return;
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
        QString qid = pressword;
        QSqlQuery query;
        query.exec("SELECT * FROM student WHERE id=="+qid+";");
        if(!query.next()){
             qDebug("信息有误，请重新输入学号");
             database.close();
             QString message = "用户: "+pressword+"不存在，请重新输入";
             QMessageBox::about(this,"提示",message);
        }
        else{
            if(query.value(5)=="普通用户"){
                        //设置打卡时间
                QString message = "用户不具有权限，请重新输入";
                QMessageBox::about(this,"提示",message);
                database.close();
            }
            else if(query.value(5)=="管理员"){
                QString message = "管理员: "+pressword+"登录成功";
                QMessageBox::about(this,"提示",message);
                this->close();
                Menu *w = new Menu();
                w->showFullScreen();
            }
            else{
                qDebug()<<query.value(5);
                database.close();
            }

       }
}

void MainWindow::on_enter_btn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("进入系统"),tr("请输入管理员号码"), QLineEdit::Password,"12345", &ok);
    if (ok && !text.isEmpty()){
        qDebug()<<text;
        pass(text);
    }

}
