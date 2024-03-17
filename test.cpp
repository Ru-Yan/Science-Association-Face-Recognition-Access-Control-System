#include "test.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <net.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/opencv.hpp>
#include "mtcnn.h"
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
using namespace cv;
using namespace std;

#define MAXFACEOPEN 0 //设置是否开关最大人脸调试，1为开，其它为关
#if CV_MAJOR_VERSION >= 3
#include "opencv2/video/video.hpp"
#endif

#include <vector>
#include "config.h"
#include "anchor_creator.h"
#include "utils.h"
#include "benchmark.h"
static int init_retinaface(ncnn::Net* retinaface, const int target_size)
{
    int ret = 0;
    retinaface->opt.num_threads = 8;
    retinaface->opt.use_winograd_convolution = true;
    retinaface->opt.use_sgemm_convolution = true;

    const char* model_param = "./model/retinaface.param";
    const char* model_model = "./model/retinaface.bin";

    ret = retinaface->load_param(model_param);
    if (ret)
    {
        return ret;
    }
    ret = retinaface->load_model(model_model);
    if (ret)
    {
        return ret;
    }

    return 0;
}

static int init_mbv2facenet(ncnn::Net* mbv2facenet, const int target_size)
{
    int ret = 0;

    mbv2facenet->opt.num_threads = 8;
    mbv2facenet->opt.use_sgemm_convolution = 1;
    mbv2facenet->opt.use_winograd_convolution = 1;

    const char* model_param = "./model/mbv2facenet.param";
    const char* model_bin = "./model/mbv2facenet.bin";

    ret = mbv2facenet->load_param(model_param);
    if (ret)
    {
        return ret;
    }

    ret = mbv2facenet->load_model(model_bin);
    if (ret)
    {
        return ret;
    }

    return 0;
}

void detect_retinaface(ncnn::Net* retinaface, cv::Mat& img, const int target_size, std::vector<cv::Mat>& face_det)
{
    int img_w = img.cols;
    int img_h = img.rows;

    const float mean_vals[3] = { 0, 0, 0 };
    const float norm_vals[3] = { 1, 1, 1 };

    ncnn::Mat input = ncnn::Mat::from_pixels(img.data, ncnn::Mat::PIXEL_BGR2RGB, img_w, img_h);
    //ncnn::Mat input = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_BGR2RGB,img_w, img_h, target_size, target_size);
    input.substract_mean_normalize(mean_vals, norm_vals);

    ncnn::Extractor ex = retinaface->create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);
    std::vector<AnchorCreator> ac(_feat_stride_fpn.size());
    for (size_t i = 0; i < _feat_stride_fpn.size(); i++)
    {
        int stride = _feat_stride_fpn[i];
        ac[i].init(stride, anchor_config[stride], false);
    }

    ex.input("data", input);

    std::vector<Anchor> proposals;

    for (size_t i = 0; i < _feat_stride_fpn.size(); i++)
    {
        ncnn::Mat cls;
        ncnn::Mat reg;
        ncnn::Mat pts;

        char cls_name[100];
        char reg_name[100];
        char pts_name[100];
        sprintf(cls_name, "face_rpn_cls_prob_reshape_stride%d", _feat_stride_fpn[i]);
        sprintf(reg_name, "face_rpn_bbox_pred_stride%d", _feat_stride_fpn[i]);
        sprintf(pts_name, "face_rpn_landmark_pred_stride%d", _feat_stride_fpn[i]);

        ex.extract(cls_name, cls);
        ex.extract(reg_name, reg);
        ex.extract(pts_name, pts);

        ac[i].FilterAnchor(cls, reg, pts, proposals);
    }

    std::vector<Anchor> finalres;
    box_nms_cpu(proposals, nms_threshold, finalres, target_size);
    //cv::resize(img, img, cv::Size(target_size, target_size));
    for (size_t i = 0; i < finalres.size(); ++i)
    {
        finalres[i].print();
        cv::Mat face = img(cv::Range((int)finalres[i].finalbox.y, (int)finalres[i].finalbox.height), cv::Range((int)finalres[i].finalbox.x, (int)finalres[i].finalbox.width)).clone();
        face_det.push_back(face);

        cv::rectangle(img, cv::Point((int)finalres[i].finalbox.x, (int)finalres[i].finalbox.y), cv::Point((int)finalres[i].finalbox.width, (int)finalres[i].finalbox.height), cv::Scalar(255, 255, 0), 2, 8, 0);
        for (size_t l = 0; l < finalres[i].pts.size(); ++l)
        {
            cv::circle(img, cv::Point((int)finalres[i].pts[l].x, (int)finalres[i].pts[l].y), 1, cv::Scalar(255, 255, 0), 2, 8, 0);
        }
    }
}

void run_mbv2facenet(ncnn::Net* mbv2facenet, std::vector<cv::Mat>& img, int target_size, std::vector<std::vector<float>>& res)
{
    for (size_t i = 0; i < img.size(); ++i)
    {
        ncnn::Extractor ex = mbv2facenet->create_extractor();
        ex.set_light_mode(true);
        ex.set_num_threads(4);
        //网络结构中的前两层已经做了归一化和均值处理， 在输入的时候不用处理了
        ncnn::Mat input = ncnn::Mat::from_pixels_resize(img[i].data, ncnn::Mat::PIXEL_BGR2RGB, img[i].cols, img[i].rows, target_size, target_size);
        ex.input("data", input);

        ncnn::Mat feat;

        ex.extract("fc1", feat);

        printf("c: %d h: %d w: %d\n", feat.c, feat.h, feat.w);
        std::vector<float> tmp;
        for (int i = 0; i < feat.w; ++i)
        {
            //printf("%f ", feat.channel(0)[i]);
            tmp.push_back(feat.channel(0)[i]);
        }
        res.push_back(tmp);
        //printf("\n");
    }
}

typedef struct masks {
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;

} masks;

static std::vector<masks> decode_infer(ncnn::Mat& data, const cv::Size& frame_size, float threshold) {
    std::vector<masks> result;
    for (int i = 0; i < data.h; i++) {
        masks box;
        const float* values = data.row(i);
        box.x1 = values[2] * (float)frame_size.width;
        box.y1 = values[3] * (float)frame_size.height;
        box.x2 = values[4] * (float)frame_size.width;
        box.y2 = values[5] * (float)frame_size.height;
        box.label = (int)values[0] - 1;
        box.score = values[1];
        if (box.score < threshold) {
            continue;
        }
        result.push_back(box);

    }
    return result;
}

int detect(cv::Mat image,float& conf_mask, float threshold=0.35, float nms_threshold=0.7){
    int img_w = image.cols;
    int img_h = image.rows;
    ncnn::Mat in_net = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR2RGB, img_w, img_h);
    float norm[3] = { 1 / 255.f, 1 / 255.f, 1 / 255.f };
    float mean[3] = { 0, 0, 0 };
    in_net.substract_mean_normalize(mean, norm);
    ncnn::Net Net;
    Net.load_param("./model/yolov4-tiny.param");
    Net.load_model("./model/yolov4-tiny.bin");
    ncnn::Extractor ex = Net.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);
    ex.input(0, in_net);
    std::vector<masks> result;
    ncnn::Mat blob;
    ex.extract("output", blob);
    vector<masks> boxes = decode_infer(blob, {img_w, img_w }, threshold);
    conf_mask = boxes[0].score;
    return boxes[0].label;
}

struct ModelConfig {
    float scale;
    float shift_x;
    float shift_y;
    int height;
    int width;
    std::string name;
    bool org_resize;
};

bool JudgeReal(cv::Mat bgr, ncnn::Extractor ex, ncnn::Net& SilentFaceSpoofing,float& conf_fake) {


    std::vector<float> cls_scores;
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR, bgr.cols, bgr.rows, 80, 80);
    fprintf(stderr, "input shape: %d %d %d %d\n", in.dims, in.h, in.w, in.c);
    ex.input("input", in);//input 是 .param文件中输入节点名称


    ncnn::Mat out;
    ex.extract("output", out);

    {
        ncnn::Layer* softmax = ncnn::create_layer("Softmax");
        ncnn::ParamDict pb;
        softmax->load_param(pb);
        softmax->forward_inplace(out, SilentFaceSpoofing.opt);
        delete softmax;
    }
    out = out.reshape(out.h * out.w * out.c);
    fprintf(stderr, "output shape: %d %d %d %d\n", out.dims, out.h, out.w, out.c);

    cls_scores.resize(out.w);
    for (int j = 0; j < out.w; j++)
    {
        cls_scores[j] = out[j];
        printf("cls_scores[%d]=%f\n", j, cls_scores[j]);
    }

    std::cout << std::endl;
    auto itMax = max_element(cls_scores.begin(), cls_scores.end());
    std::cout << "the max:" << *itMax << " the location:" << distance(cls_scores.begin(), itMax) << std::endl;
    if (distance(cls_scores.begin(), itMax) == 1) {
        std::cout << "Image " << distance(cls_scores.begin(), itMax) << "   is Real Face. Score:  " << *itMax << std::endl;
        conf_fake = *itMax;
        return true;
    }

    else {
        std::cout << "Image " << distance(cls_scores.begin(), itMax) << "   is Fake Face. Score:  " << *itMax << std::endl;
        conf_fake = *itMax;
        return false;
    }
}

void compares(cv::Mat& img1,string& name,float& conf_fake,float& conf_mask,float& conf_face)
{
    int target_size = 300;
    int facenet_size = 112;
    ncnn::Net retinaface;
    ncnn::Net mbv2facenet;

    int ret = 0;

    std::vector<cv::String> imagePathList;
    cv::glob(".\\data", imagePathList);

    for (int i = 0; i < imagePathList.size(); i++)
    {
        std::cout << imagePathList[i] << "\n";
        cv::Mat img2 = cv::imread(imagePathList[i]);
        ret = init_retinaface(&retinaface, target_size);
        ret = init_mbv2facenet(&mbv2facenet, facenet_size);
        cv::resize(img1, img1, cv::Size(target_size, target_size));
        cv::resize(img2, img2, cv::Size(target_size, target_size));
        std::vector<cv::Mat> face_det;
        std::vector<std::vector<float>> feature_face;
        detect_retinaface(&retinaface, img1, target_size, face_det); //pic1 dect
        detect_retinaface(&retinaface, img2, target_size, face_det);//pic2 dect
        if (face_det.size() == 2) //只有两张人脸的时候才进行人脸识别
        {
            //活体检测
            static ncnn::Net SilentFaceSpoofing;
            SilentFaceSpoofing.load_param("./model/2.7_80x80_MiniFASNetV2.param");
            SilentFaceSpoofing.load_model("./model/2.7_80x80_MiniFASNetV2.bin");
            static ncnn::Extractor ex = SilentFaceSpoofing.create_extractor();
            bool result;
            result = JudgeReal(img1, ex, SilentFaceSpoofing,conf_fake);
            if (result == false) {
               name = "faked";
               return name;
            }
            //口罩检测
            int is_wear = detect(img1,conf_mask);
            if (is_wear == 1) {
                name = "marked";
                return name;
            }

            run_mbv2facenet(&mbv2facenet, face_det, facenet_size, feature_face);

            //余弦距离sim值越接近1，代表两个向量的夹角越接近0，则两个向量越相似。反之，越接近0，代表两个向量夹角趋于90°，两个向量差异越大。
            //相似阈值可以取 > 0.3
            float sim = calc_similarity_with_cos(feature_face[0], feature_face[1]);
            conf_face = sim;
            char text[50];
            if (sim >= 0.3)
            {
                cv::putText(img1, text, cv::Point(target_size - 250, target_size - 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
                printf("face_sim: %f\n", sim);//置信度

                cv::Point origin;
                origin.x = img1.rows / 2 - img1.rows / 2;
                origin.y = img1.cols / 2 + img1.cols / 2;


                name = imagePathList[i];
                int size = name.size();
                name = name.substr(7, size);
                size = name.size();
                int i = 0;
                for (i; i < size; i++)
                {
                    if (name[i] == '-')
                    {
                        break;
                    }
                }
                name = name.substr(0,i);//姓名
                break;
            }
        }
    }
    return name;
}

void rec_face(cv::Mat& img1,string& name,float& conf_fake,float& conf_mask,float& conf_face) {
    compares(img1,name,conf_fake,conf_mask,conf_face);
    return name;
}

void add_face(string name,cv::Mat img) {
    const char* model_path = "./model";
    MTCNN mtcnn(model_path);
    string save_path = "./data",label;
    mtcnn.GetSamples(name,img);
    return;
}
