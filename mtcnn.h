//
// Created by Lonqi on 2017/11/18.
//
#pragma once

#ifndef __MTCNN_NCNN_H__
#define __MTCNN_NCNN_H__
#include "net.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
#include <map>
#include <iostream>
using namespace std;
using namespace cv;
struct Bbox
{
    float score;
    int x1;
    int y1;
    int x2;
    int y2;
    float area;
    float ppoint[10];
    float regreCoord[4];
};

struct facebox
{
    float face[128];
};

typedef struct BoxInfo {
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;

} BoxInfo;

class MTCNN {

public:
    MTCNN(const string &model_path);
    MTCNN(const std::vector<std::string> param_files, const std::vector<std::string> bin_files);
    ~MTCNN();

    void SetMinFace(int minSize);
    void detect(ncnn::Mat& img_, std::vector<Bbox>& finalBbox);
    void GetNames(string save_path, string& name);
    void GetSamples(string label,cv::Mat img);
    void detectMaxFace(ncnn::Mat& img_, std::vector<Bbox>& finalBbox);
    std::vector<ncnn::Mat> face_vec;
  //  void detection(const cv::Mat& img, std::vector<cv::Rect>& rectangles);
private:
    void generateBbox(ncnn::Mat score, ncnn::Mat location, vector<Bbox>& boundingBox_, float scale);
    void nmsTwoBoxs(vector<Bbox> &boundingBox_, vector<Bbox> &previousBox_, const float overlap_threshold, string modelname = "Union");
    void nms(vector<Bbox> &boundingBox_, const float overlap_threshold, string modelname="Union");
    void refine(vector<Bbox> &vecBbox, const int &height, const int &width, bool square);
    void extractMaxFace(vector<Bbox> &boundingBox_);

    void PNet(float scale);
    void PNet();
    void RNet();
    void ONet();

    void compare_face(vector<string>& matches, vector<float>& distances);
    vector<float>& face_distance(vector<float>& know_face, vector<float>& face);
    vector<BoxInfo> decode_infer(ncnn::Mat& data, const cv::Size& frame_size, float threshold);
    void g_r(std::vector<BoxInfo>& input_boxes, float NMS_THRESH);

    ncnn::Net Pnet, Rnet, Onet;
    ncnn::Mat img;

    const float nms_threshold[3] = {0.5f, 0.7f, 0.7f};
    const float mean_vals[3] = {127.5, 127.5, 127.5};
    const float norm_vals[3] = {0.0078125, 0.0078125, 0.0078125};
    const int MIN_DET_SIZE = 12;
    std::vector<Bbox> firstPreviousBbox_, secondPreviousBbox_, thirdPrevioussBbox_;
    std::vector<Bbox> firstBbox_, secondBbox_,thirdBbox_;
    facebox facebox_;
    int img_w, img_h;


private://部分可调参数
    const float threshold[3] = { 0.8f, 0.8f, 0.6f };
    int minsize = 40;
    const float pre_facetor = 0.709f;

};


#endif //__MTCNN_NCNN_H__
