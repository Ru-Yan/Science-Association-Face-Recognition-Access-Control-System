#include "teacher.h"
#include "ui_teacher.h"
#include "mainwindow.h"
#include <QinputDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <maininterface.h>
teacher::teacher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teacher)
{
    ui->setupUi(this);
    ui->student->setColumnWidth(4,200);
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
    QString sql = QString("SELECT * FROM student WHERE state=='管理员'");
    query.exec(sql);
    QString id;
    QString name;
    QString class_2;
    QString school;
    int cow=0;
    while(query.next()){
        for(int row=0;row<4;row++){
            ui->student->setItem(cow,row,new QTableWidgetItem(query.value(row).toString()));
        }
        cow++;
    }
    database.close();
}

teacher::~teacher()
{
    delete ui;
}

void teacher::on_backbt_clicked()
{
    MainInterface *m = new MainInterface();
    m->showFullScreen();
    this->close();
}

void teacher::on_deletebt_clicked()
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
        QString sql_id = ui->titleclearedit->text();
        QSqlQuery sql_query;
        sql_query.exec("UPDATE student SET state=='普通用户' WHERE id=="+sql_id+" AND state=='管理员';");
        teacher *w = new teacher();
        w->showFullScreen();
        this->close();
}

void teacher::on_findbt_clicked()
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
        QString sql_id = ui->noedit->text();
        QSqlQuery sql_query;
        sql_query.exec("SELECT * FROM student WHERE id=="+sql_id+";");
        while(sql_query.next())
        {
            QString id = sql_query.value(0).toString();
            QString name = sql_query.value(1).toString();
            QString class_2 = sql_query.value(2).toString();
            QString school = sql_query.value(3).toString();
            qDebug()<<id<<name<<class_2<<school;
            ui->listWidget->addItem(id+" "+name+" "+class_2+" "+school);
        }
}

void teacher::on_setbt_clicked()
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
        QString sql_id = ui->noedit->text();
        QSqlQuery sql_query;
        sql_query.exec("UPDATE student SET state=='管理员' WHERE id=="+sql_id+";");
        qDebug() << "Succeed to set..." ;
        teacher *w = new teacher();
        w->showFullScreen();
        this->close();
}


void teacher::on_refresh_clicked()
{
    teacher *w = new teacher();
    w->showFullScreen();
    this->close();
}
