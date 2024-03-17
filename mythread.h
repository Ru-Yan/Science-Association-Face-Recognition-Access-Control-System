#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>
#include "test.h"
class FaceRecog : public QThread
{
   Q_OBJECT
public:
   explicit FaceRecog(QObject *parent = 0);

signals:
   void recface_ok(QString message);
   void recManger_ok(QString message);

protected:
    void run() override;
};

#endif // MYTHREAD_H
