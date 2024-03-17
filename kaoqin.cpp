#include "kaoqin.h"
#include"mainwindow.h"
#include "ui_kaoqin.h"
#include <QDebug>
#include "maininterface.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <net.h>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QCryptographicHash>  //md5加密的库

Kaoqin::Kaoqin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Kaoqin)
{

    ui->setupUi(this);
    ui->studaka->setColumnWidth(4,200);
    m_NetManger = new QNetworkAccessManager;
    QUrl url("http://hzeen.cn:5080/api/Reserve/getAllReserve");
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
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("yyyy-MM-dd");
            QString Time = str+"%";
            QString sql = QString("SELECT * FROM student where time like '%1'").arg(Time);
            query.exec(sql);
            int cow=0;
            int num=0;
            int uncow=0;
            while(query.next()){
                for(int row=0;row<5;row++){
                    ui->studaka->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
                    qDebug() << row ;
                }
                cow++;
            }
            qDebug() <<cow;
            QString a = QString::number(cow);
            ui->lineEdit->setText(a);
            QSqlQuery sql_query;
            QString sql2= QString("SELECT * FROM student");
            sql_query.exec(sql2);
            while(sql_query.next()){
                num++;
            }

//            uncow=num-cow;
//            QString b = QString::number(uncow);
//            ui->lineEdit_2->setText(b);
            database.close();           
            QString c = QString::number(count/8);
            ui->yuyue_line->setText(c);

}

Kaoqin::~Kaoqin()
{
    delete ui;
}


void Kaoqin::on_backbt_clicked()
{
    MainInterface *m = new MainInterface();
    m->show();
    m->showFullScreen();
    this->close();
}

void Kaoqin::on_refresh_clicked()
{
    Kaoqin *m = new Kaoqin();
    m->show();
    m->showFullScreen();
    this->close();
}
void Kaoqin::finishedSlot(QNetworkReply *)
{
//    recieveData
    QString error = m_Reply->errorString();
        if (!error.isEmpty()) {
            qDebug()<< error;
        }

        if (m_Reply->bytesAvailable() > 0 && m_Reply->error() == QNetworkReply::NoError) {
            count=0;
            QString data = m_Reply->readAll();
            m_Reply->deleteLater();

            qDebug()<< data;
            //初始化脚本引擎
            QScriptEngine engine;
            //构建解析对象
            QScriptValue script = engine.evaluate("value=" + data);
                   if(script.isArray()) {
                       int i=1;
                       if(!script.property(i).isNull()) {
                       QString stu_id,stu_name;
                       //创建迭代器逐个解析具体值
                       QScriptValueIterator it(script);
                       while (it.hasNext()) {
                           it.next();
                           stu_id = it.value().property("stu_id").toString();
                           stu_name = it.value().property("stu_name").toString();
                           qDebug()<<stu_id<<stu_name;
                           count++;
                           qDebug()<< count;
                       }

                      i++;

                     }
                   }
    }
}


