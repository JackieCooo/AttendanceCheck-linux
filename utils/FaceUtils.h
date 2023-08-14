#pragma once

#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <dlib/dnn.h>

#include "Global.h"

using namespace dlib;
using namespace std;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
        alevel0<
        alevel1<
        alevel2<
        alevel3<
        alevel4<
        max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
            input_rgb_image_sized<150>
        >>>>>>>>>>>>;

class FaceUtils {
public:
    FaceUtils();
    int detectFace(matrix<dlib::rgb_pixel>& mat, full_object_detection& face, bool checkBound = false);
    void getFaceDescriptor(matrix<dlib::rgb_pixel>& mat, full_object_detection& face, matrix<float, 0, 1>& des);
    void saveFaceData(const std::string& name, matrix<float, 0, 1>& des);
    int recogniseFace(matrix<float, 0, 1>& tar, string& name);

private:
    shape_predictor predictor;
    frontal_face_detector detector;
    anet_type net;
    static constexpr float DEFAULT_THRESHOLD = 0.5;

    static bool checkRectInBound(const dlib::rectangle& rect);
};

extern FaceUtils faceUtils;
