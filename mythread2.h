#ifndef MYTHREAD2_H
#define MYTHREAD2_H
#include <QThread>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>
#include "test.h"
class FaceAdd : public QThread
{
   Q_OBJECT
public:
   explicit FaceAdd(QObject *parent = 0);

signals:
    void add_over(QString message);
public slots:
    void setId(String id);
protected:
    void run() override;
private:
    String face_id;
};

#endif // MYTHREAD_H
