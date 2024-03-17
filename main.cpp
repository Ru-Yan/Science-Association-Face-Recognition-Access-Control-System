#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    //设置中文编码
    #if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    #if _MSC_VER
        QTextCodec *codec = QTextCodec::codecForName("GBK");
    #else
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    #endif
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    #else
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
    #endif
//    qRegisterMetaType<string>("string");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showFullScreen();
    return a.exec();
}
