#include "notice.h"
#include "ui_notice.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include "maininterface.h"
#include <QTextEdit>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QCryptographicHash>  //md5加密的库

Notice::Notice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notice)
{
    ui->setupUi(this);
    ui->tableView1->setColumnWidth(3,50);
    m_NetManger = new QNetworkAccessManager;
    QUrl url("http://hzeen.cn:5080/api/acti/getallacti");
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
                QSqlQuery sql_query;
                    QString sql = QString("SELECT * FROM acti");
                    query.exec(sql);
                    int cow=0;
                    while(query.next()){
                    QDateTime time = QDateTime::currentDateTime();
                    int ctime = time.toTime_t();
                    QString sstr = query.value(4).toString();
                    QString estr = query.value(5).toString();
                    QDateTime st = QDateTime::fromString(sstr, "yyyy-MM-dd");
                    QDateTime et = QDateTime::fromString(estr, "yyyy-MM-dd");
                    int stime = st.toTime_t();
                    int etime = et.toTime_t();
                    if(ctime>etime) {
                        QString sql2 = QString("UPDATE acti SET acti_state='已完结' WHERE start_time='%1' AND end_time='%2'").arg(sstr).arg(estr);
                        sql_query.exec(sql2);
                    }
                    else if(ctime>stime&&ctime<etime) {
                        QString sql3 = QString("UPDATE acti SET acti_state='进行中' WHERE start_time='%1' AND end_time='%2'").arg(sstr).arg(estr);
                        sql_query.exec(sql3);
                    }
                ui->tableView1->setItem(cow,0,new QTableWidgetItem(query.value(0).toString()));
                ui->tableView1->setItem(cow,1,new QTableWidgetItem(query.value(1).toString()));
                ui->tableView1->setItem(cow,2,new QTableWidgetItem(query.value(3).toString()));
                ui->tableView1->setItem(cow,3,new QTableWidgetItem(query.value(2).toString()));
                cow++;
            }
            database.close();
}
Notice::~Notice()
{
    delete ui;
}
//将通知消息保存到数据库
void Notice::on_sendbt_clicked()
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


            if(!sql_query.exec("create table IF NOT EXISTS  acti(acti_head varchar(255), acti_content varchar(255), acti_peo varchar(255), acti_state varchar(255)),start_time date,end_time date"))
            {
                qDebug() << "Error: Fail to create table."<< sql_query.lastError();
            }
            else
            {
                qDebug() << "notice Table created!";
            }

            QString arg[6]={ui->titleedit->text(),ui->messageedit->toPlainText(),ui->notice_name->text(),"未开始",ui->start_time->text(),ui->end_time->text()};
            QSqlQuery query;
            QString sql = QString("insert into acti values ('%1','%2','%3','%4','%5','%6')").arg(arg[0]).arg(arg[1]).arg(arg[2]).arg(arg[3]).arg(arg[4]).arg(arg[5]);
            query.prepare(sql);
            if(!query.exec(sql)){
                qDebug() << "Database inserted Failed！"<< sql_query.lastError();
                database.close();

            }
            else{
                QString message = "消息添加成功";
                QMessageBox::about(this,"提示",message);

                qDebug() << "Database inserted successfully！";
                database.close();

            }
            Notice *m = new Notice();
            m->showFullScreen();
            this->close();
}


//返回主界面
void Notice::on_backbt_clicked()
{
    MainInterface *m = new MainInterface();
    m->showFullScreen();
    this->close();
}

//通过标题对消息进行删除
void Notice::on_deletebt_clicked()
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
    QString headline=ui->titleclearedit->text();
    QSqlQuery query;
    QString sql="DELETE FROM acti WHERE acti_head="+headline+";";
    if(!query.exec(sql)){
        QString message = "消息不存在";
        QMessageBox::about(this,"提示",message);
        database.close();

    }
    else{
        QString message = "消息删除成功";
        QMessageBox::about(this,"提示",message);

        qDebug() << "Database deleted successfully！";
        database.close();

    }
    Notice *m = new Notice();
    m->showFullScreen();
    this->close();

}
void Notice::finishedSlot(QNetworkReply *)
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
            if(!sql_query.exec("create table IF NOT EXISTS  acti(acti_head varchar(255), acti_content varchar(255), acti_peo varchar(255),acti_state varchar(255),start_time date, end_time date)"))
          {
              qDebug() << "Error: Fail to create table."<< sql_query.lastError();
          }
          else
          {
              qDebug() << "acti Table created!";
          }
            //初始化脚本引擎
            QScriptEngine engine;
            //构建解析对象
            QScriptValue script = engine.evaluate("value=" + data);
                   if(script.isArray()) {
                       int i=1;
                       if(!script.property(i).isNull()) {
                       QString acti_head,acti_content,acti_peo,acti_state,start_time,end_time;
                       //创建迭代器逐个解析具体值
                       QScriptValueIterator it(script);
                       while (it.hasNext()) {
                           it.next();
                           acti_head = it.value().property("acti_head").toString();
                           acti_content = it.value().property("acti_content").toString();
                           acti_peo = it.value().property("acti_peo").toString();
                           acti_state = it.value().property("acti_state").toString();
                           start_time = it.value().property("start_time").toString().left(10);
                           end_time = it.value().property("end_time").toString().left(10);
                           qDebug()<<acti_head<<acti_content<<acti_peo<<acti_state<<start_time<<end_time;
                           QString sql = QString("INSERT INTO ACTI VALUES('%1','%2','%3','%4','%5','%6')").arg(acti_head).arg(acti_content).arg(acti_peo).arg(acti_state).arg(start_time).arg(end_time);
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

