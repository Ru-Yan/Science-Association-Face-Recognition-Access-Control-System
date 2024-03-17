#include "maininterface.h"
#include "ui_maininterface.h"
#include "menu.h"
#include "kaoqin.h"
#include "notice.h"
#include "teacher.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <qDebug>
#include "mainwindow.h"
MainInterface::MainInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);
}

MainInterface::~MainInterface()
{
    delete ui;
}

//设置是哪个管理员登录进来
void MainInterface::setAdmin(QString name)
{
    this->Admin_name = name;
}


//学生信息管理
void MainInterface::on_stubt_clicked()
{
    Menu *w = new Menu();
    w->showFullScreen();
    this->close();
}

//考勤统计
void MainInterface::on_recordbt_clicked()
{
    Kaoqin *w = new Kaoqin();
    w->showFullScreen();
    this->close();
}
//写通知栏
void MainInterface::on_noticebt_clicked()
{
    Notice *w =new Notice();
    w->showFullScreen();
    this->close();
}

//管理员信息管理
void MainInterface::on_edmitbt_clicked()
{
    teacher *w = new teacher();
    w->showFullScreen();
    this->close();
}
//返回登录界面
void MainInterface::on_backbt_clicked()
{
    MainWindow *w = new MainWindow();
    w->showFullScreen();
    this->close();
}
