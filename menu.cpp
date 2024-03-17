#include "menu.h"
#include "ui_menu.h"
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include <QProcess>
#include <QFileDialog>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QListView>
#include <QStandardItemModel>
#include <QBitmap>
#include <QPainter>
#include <QImage>
#include "test.h"
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QCryptographicHash>  //md5加密的库
using namespace std;
using namespace cv;
extern cv::VideoCapture capture2;

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    FA = new FaceAdd;
    connect(&theTimer2, SIGNAL(timeout()), this, SLOT(run2()));
    connect(this,SIGNAL(addface_begin(String)),FA,SLOT(setId(String)));
    connect(FA,SIGNAL(add_over(QString)),this,SLOT(addover_tips(QString)));

    ui->school->setView(new  QListView());
    ui->class_2->setView(new  QListView());
    ui->label_state->setText(" ");
    ui->addbt->setEnabled(false);//禁用
    m_NetManger = new QNetworkAccessManager;
    QUrl url("http://hzeen.cn:5080/api/user/getalluser");
    m_Reply = m_NetManger->get(QNetworkRequest(url));
    QObject::connect(m_NetManger, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply*)));
    //打开数据库
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

        if (!database.open())
        {
            qDebug() << "Error: Failed to connect database..." << database.lastError();

        }
        else
        {
            qDebug() << "Succeed to connect database..." ;

        }

    QSqlQuery query;
    QString sql = QString("SELECT * FROM student");
    query.exec(sql);
    int cow=0;
    while(query.next()){
        for(int row=0;row<4;row++){
            ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
        }
        cow++;
    }
    database.close();
}

Menu::~Menu()
{
    if(capture2.isOpened())
        capture2.release();
    delete ui;
}
void Menu::addover_tips(QString message){
    cout<<"get"<<endl;
    theTimer2.stop();
    capture2.release();

    FA->quit();
    FA->wait();
    FA->deleteLater();

    QMessageBox::about(this,"提示",message);
    ui->addbt->setEnabled(true);
}
bool Menu::can_add(){
    //建立并打开数据库
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

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database..." << database.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database..." ;
    }
   //创建表格
    QSqlQuery sql_query;
    if(!sql_query.exec("create table IF NOT EXISTS  student(id varchar(20) primary key, name varchar(20), class_2 varchar(20), school varchar(20),time Text, state varchar(20))"))
    {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    }
    else
    {
        qDebug() << "student Table created!";
    }
    QString qid = ui->number->text();
    if(ui->number->text()==""||ui->name->text()==""||ui->school->currentIndex()==0){
        QMessageBox::about(this,"提示","请先填写好信息");
        return false;
    }
    QSqlQuery query;
    query.exec("SELECT * FROM student WHERE id=="+qid+";");
    if(query.next()){
        database.close();
        QString message = "用户: "+qid+"已存在";
        QMessageBox::about(this,"提示",message);
        return false;
    }
    else{
        database.close();
        return true;
    }

}

//点击 打开摄像头 按钮
void Menu::on_openbt_clicked()
{
    if(can_add()){
        if (capture2.open(200)) {
                cout<<"cam2 is  opened"<<endl;
                //从摄像头捕获视频
                theTimer2.start(300);
                //实例化人脸识别线程
                String stu_id=ui->number->text().toStdString();
                emit addface_begin(stu_id);
                ui->label_state->setText("正在采集人脸信息,请稍等....");
                FA->start();

        }else {
            cout<<"cam2 is not opened"<<endl;
            theTimer2.stop();
            capture2.release();
        }
    }
}
void Menu::run2()
{
    //获取图片
    capture2>>srcImage2;
    //显示
    cvtColor(srcImage2,srcImage2,CV_BGR2RGB);
    QImage image = QImage((uchar*)(srcImage2.data), srcImage2.cols, srcImage2.rows, srcImage2.step1(), QImage::Format_RGB888);
    QPixmap mmp=QPixmap::fromImage(image);
    mmp =  PixmapToRound(mmp, 100);
    ui->add_face->setPixmap(mmp);
    ui->stu_face->hide();
}


//将图片切成圆形
QPixmap Menu::PixmapToRound(const QPixmap &src, int radius)
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

//添加学生信息
void Menu::on_addbt_clicked()
{
    //建立并打开数据库
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

            if (!database.open())
            {
                qDebug() << "Error: Failed to connect database..." << database.lastError();
            }
            else
            {
                qDebug() << "Succeed to connect database..." ;
            }

            //创建表格
            QSqlQuery sql_query;


                  if(!sql_query.exec("create table IF NOT EXISTS  student(id varchar(20) primary key, name varchar(20), class_2 varchar(20), school varchar(20),time Text, state varchar(20))"))
                {
                    qDebug() << "Error: Fail to create table."<< sql_query.lastError();
                }
                else
                {
                    qDebug() << "student Table created!";
                }
                QDateTime time = QDateTime::currentDateTime();
                QString str = time.toString("yyyy-MM-dd hh:mm");

                QString state;
                state="普通用户";
                QString arg[6]={ui->number->text(),ui->name->text(),ui->class_2->currentText(),ui->school->currentText(),str,state};
                QSqlQuery query;
                QString sql = QString("insert into student values ('%1','%2','%3','%4','%5','%6')").arg(arg[0]).arg(arg[1]).arg(arg[2]).arg(arg[3]).arg(arg[4]).arg(arg[5]);
                query.prepare(sql);
                if(!query.exec(sql)){
                    qDebug() << "Database inserted Failed！"<< sql_query.lastError();
                    database.close();

                }
                else{
                    QString message = "用户添加成功";
                    QMessageBox::about(this,"提示",message);
                    qDebug() << "Database inserted successfully！";
                    database.close();

                    Menu *w = new Menu();
                    w->showFullScreen();
                    this->close();

                }

}
//返回主界面
void Menu::on_backbt_clicked()
{
    MainInterface *m = new MainInterface();
    m->showFullScreen();
    this->close();
}



void Menu::on_school_currentIndexChanged(int index)
{

    switch (index) {
    case 0:
        ui->class_2->clear();
        break;
    case 1:
        ui->class_2->clear();
        ui->class_2->addItem(tr("物联网一班"));
        ui->class_2->addItem(tr("物联网二班"));
        break;
    case 2:
        ui->class_2->clear();
        ui->class_2->addItem(tr("会计一班"));
        ui->class_2->addItem(tr("会计二班"));
        break;
    case 3:
        ui->class_2->clear();
        ui->class_2->addItem(tr("计算机科学与技术一班"));
        ui->class_2->addItem(tr("软件工程一班"));
        break;
    default:
        break;
    }


}

void Menu::on_find_student_clicked()
{
    //建立并打开数据库
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

        if (!database.open())
        {
            qDebug() << "Error: Failed to connect database..." << database.lastError();
        }
        else
        {
            qDebug() << "Succeed to connect database..." ;
        }

        QString sql_id = ui->clear_student->text();
        QSqlQuery sql_query;
        sql_query.exec("SELECT * FROM student WHERE id=="+sql_id+";");
        if(sql_query.next())
        {
               QString id = sql_query.value(0).toString();
               QString name = sql_query.value(1).toString();
               QString class_2 = sql_query.value(2).toString();
               QString school = sql_query.value(3).toString();
               qDebug()<<id<<name<<class_2<<school;

               int back = QMessageBox::question(this,"提示","查找成功，是否删除？",QMessageBox::Ok,QMessageBox::No);
               if(back == QMessageBox::Ok)
               {
                        sql_query.exec("DELETE FROM student WHERE id=="+sql_id+";");
                        QMessageBox::about(this,"提示","删除成功");
                        qDebug( "删除成功" );
                        Menu *w = new Menu();
                        w->showFullScreen();
                        this->close();
               }
        }else{
            QMessageBox::about(this,"提示","不存在此学生");
        }
}

void Menu::on_check_xuyuan_currentIndexChanged(const QString &arg1)
{
    //打开数据库
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
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database..." << database.lastError();

    }
    else
    {
        qDebug() << "Succeed to connect database..." ;

    }

    if(ui->check_xuyuan->currentText() == "物联网工程学院")
    {
        ui->check_class->clear();
        ui->check_class->addItem(tr(""));
        ui->check_class->addItem(tr("物联网一班"));
        ui->check_class->addItem(tr("物联网二班"));

        ui->student->clear();
        QSqlQuery query;
        QString sql = QString("SELECT * FROM student WHERE school=='物联网工程学院'");
        query.exec(sql);
        int cow=0;
        while(query.next()){
            for(int row=0;row<4;row++){
                ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
            }
            cow++;
        }
        database.close();

    }
    else if(ui->check_xuyuan->currentText()== "商学院")
    {
        ui->check_class->clear();
        ui->check_class->addItem(tr(""));
        ui->check_class->addItem(tr("会计一班"));
        ui->check_class->addItem(tr("会计二班"));

        ui->student->clear();
        QSqlQuery query;
        QString sql = QString("SELECT * FROM student WHERE school=='商学院'");
        query.exec(sql);
        int cow=0;
        while(query.next()){
            for(int row=0;row<4;row++){
                ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
            }
            cow++;
        }
        database.close();

    }
    else if(ui->check_xuyuan->currentText()== "计算机学院")
    {
        ui->check_class->clear();
        ui->check_class->addItem(tr(""));
        ui->check_class->addItem(tr("计算机科学与技术一班"));
        ui->check_class->addItem(tr("软件工程一班"));

        ui->student->clear();
        QSqlQuery query;
        QString sql = QString("SELECT * FROM student WHERE school=='计算机学院'");
        query.exec(sql);
        int cow=0;
        while(query.next()){
            for(int row=0;row<4;row++){
                ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
            }
            cow++;
        }
        database.close();

    }

}

void Menu::on_check_class_currentIndexChanged(const QString &arg1)
{

    //打开数据库
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
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database..." << database.lastError();

    }
    else
    {
        qDebug() << "Succeed to connect database..." ;

    }
    if(ui->check_xuyuan->currentText() == "物联网工程学院"){
        if(ui->check_class->currentText() == "物联网一班")
        {
            ui->student->clear();
            QSqlQuery query;
            QString sql = QString("SELECT * FROM student WHERE class_2=='物联网一班'");
            query.exec(sql);
            int cow=0;
            while(query.next()){
                for(int row=0;row<4;row++){
                    ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                }
                cow++;
            }
            database.close();

        }
        else if(ui->check_class->currentText()== "物联网二班")
        {
            ui->student->clear();
            QSqlQuery query;
            QString sql = QString("SELECT * FROM student WHERE class_2=='物联网二班'");
            query.exec(sql);
            int cow=0;
            while(query.next()){
                for(int row=0;row<4;row++){
                    ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                }
                cow++;
            }
            database.close();
        }
    }
    else if(ui->check_xuyuan->currentText() == "商学院"){
        if(ui->check_class->currentText() == "会计一班")
        {
            ui->student->clear();
            QSqlQuery query;
            QString sql = QString("SELECT * FROM student WHERE class_2=='会计一班'");
            query.exec(sql);
            int cow=0;
            while(query.next()){
                for(int row=0;row<4;row++){
                    ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                }
                cow++;
            }
            database.close();

        }
        else if(ui->check_class->currentText()== "会计二班")
        {
            ui->student->clear();
            QSqlQuery query;
            QString sql = QString("SELECT * FROM student WHERE class_2=='会计二班'");
            query.exec(sql);
            int cow=0;
            while(query.next()){
                for(int row=0;row<4;row++){
                    ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                }
                cow++;
            }
            database.close();
        }
    }
    else if(ui->check_xuyuan->currentText() == "计算机学院"){
        if(ui->check_class->currentText() == "计算机科学与技术一班")
        {
            ui->student->clear();
            QSqlQuery query;
            QString sql = QString("SELECT * FROM student WHERE class_2=='计算机科学与技术一班'");
            query.exec(sql);
            int cow=0;
            while(query.next()){
                for(int row=0;row<4;row++){
                    ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                }
                cow++;
            }
            database.close();

        }
        else if(ui->check_class->currentText()== "软件工程一班")
        {
            ui->student->clear();
            QSqlQuery query;
            QString sql = QString("SELECT * FROM student WHERE class_2=='软件工程一班'");
            query.exec(sql);
            int cow=0;
            while(query.next()){
                for(int row=0;row<4;row++){
                    ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                }
                cow++;
            }
            database.close();
        }
    }
}
void Menu::finishedSlot(QNetworkReply *)
{

    QString error = m_Reply->errorString();
        if (!error.isEmpty()) {
            qDebug()<< error;
        }

        if (m_Reply->bytesAvailable() > 0 && m_Reply->error() == QNetworkReply::NoError) {
            QString data = m_Reply->readAll();
            m_Reply->deleteLater();

            //发送接收数据信号
            qDebug()<< data;
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
            QSqlQuery sql_query;
            if(!sql_query.exec("create table IF NOT EXISTS  student(id varchar(20) primary key, name varchar(20), class_2 varchar(20), school varchar(20),time Text, state varchar(20))"))
          {
              qDebug() << "Error: Fail to create table."<< sql_query.lastError();
          }
          else
          {
              qDebug() << "Table created!";
          }
            //初始化脚本引擎
            QScriptEngine engine;
            //构建解析对象
            QScriptValue script = engine.evaluate("value=" + data);
                   if(script.isArray()) {
                       int i=1;
                       if(!script.property(i).isNull()) {
                       QString stu_id,stu_name,stu_major,stu_class,state;

                       //创建迭代器逐个解析具体值
                       QScriptValueIterator it(script);
                       while (it.hasNext()) {
                           it.next();
                           stu_id = it.value().property("stu_id").toString();
                           stu_name = it.value().property("stu_name").toString();
                           stu_major = it.value().property("stu_major").toString();
                           stu_class = it.value().property("stu_class").toString();
                           state="普通用户";
                           qDebug()<<stu_id<<stu_name<<stu_major<<stu_class;
                           QString arg[4]={stu_id,stu_name,stu_class,stu_major};
                           QSqlQuery sql_query;
                           QString sql = QString("insert into student(id, name,class_2, school,state) values ('%1','%2','%3','%4','%5')").arg(arg[0]).arg(arg[1]).arg(arg[2]).arg(arg[3]).arg(state);
                           sql_query.prepare(sql);
                           if(!sql_query.exec(sql)){
                               qDebug() << "Database inserted Failed！"<< sql_query.lastError();


                           }
                           else{
                               qDebug() << "Database inserted successfully！";


                           }
                       }
                      i++;
                     }
                       database.close();
                   }


}
}
