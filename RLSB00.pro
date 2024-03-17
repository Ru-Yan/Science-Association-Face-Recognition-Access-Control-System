#-------------------------------------------------
#
# Project created by QtCreator 2022-05-07T16:11:12
#
#-------------------------------------------------

QT       += core gui sql
QT+=script
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RLSB00
TEMPLATE = app

# 配置 openmp
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    kaoqin.cpp \
    notice.cpp\
    maininterface.cpp \
    menu.cpp \
    teacher.cpp \
    mtcnn.cpp \
    utils.cpp \
    config.cpp \
    anchor_creator.cpp \
    mythread.cpp \
    test.cpp \
    mythread2.cpp
HEADERS  += mainwindow.h \
    kaoqin.h \
    notice.h\
    maininterface.h \
    menu.h \
    teacher.h \
    mtcnn.h \
    utils.h \
    config.h \
    anchor_creator.h \
    mark.id.h \
    mythread.h \
    test.h \
    mythread2.h

FORMS    += mainwindow.ui \
    kaoqin.ui \
    notice.ui\
    maininterface.ui \
    menu.ui \
    teacher.ui

QMAKE_CXXFLAGS+= -std=c++11

#opencv
INCLUDEPATH += $$PWD/opencv86/include
LIBS += -L$$PWD/opencv86/lib -lopencv_videoio -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_imgcodecs -lopencv_imgproc -lopencv_videostab -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_video -lopencv_superres -lopencv_objdetect -lopencv_flann -lopencv_features2d -lopencv_calib3d -latomic
LIBS += -L$$PWD/opencv86 -ljpeg -lv4l2
DEPENDPATH += $$PWD/opencv86/lib

#ncnn
INCLUDEPATH += $$PWD/libncnn/include
LIBS += -L$$PWD/libncnn -lncnn


RESOURCES += \
    res.qrc






