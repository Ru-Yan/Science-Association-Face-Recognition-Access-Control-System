#ifndef TEST_H
#define TEST_H

#include "net.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
#include <map>
#include <iostream>
#include <QWidget>
using namespace std;
using namespace cv;

static int init_retinaface(ncnn::Net* retinaface, const int target_size);
static int init_mbv2facenet(ncnn::Net* mbv2facenet, const int target_size);
void detect_retinaface(ncnn::Net* retinaface, cv::Mat& img, const int target_size, std::vector<cv::Mat>& face_det);
void run_mbv2facenet(ncnn::Net* mbv2facenet, std::vector<cv::Mat>& img, int target_size, std::vector<std::vector<float>>& res);
void compares(cv::Mat& img1,string& name,float& conf_fake,float& conf_mask,float& conf_face);
void add_face(string name,cv::Mat img);
void rec_face(cv::Mat& img1,string& name,float& conf_fake,float& conf_mask,float& conf_face);
bool JudgeReal(cv::Mat bgr, ncnn::Extractor ex, ncnn::Net& SilentFaceSpoofing,float& conf_fake);
int detect(cv::Mat image,float& conf_mask, float threshold=0.35, float nms_threshold=0.7);

#endif // TEST_H
