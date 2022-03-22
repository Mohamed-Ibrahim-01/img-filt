#include "ImgProc.h"
#include "histogramEqualization.h"

#include "opencv2/imgproc/imgproc.hpp"

void OpenCvImgProc::gaussianFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::GaussianBlur(src, dst, cv::Size(5, 5), 0, 0);
}

void OpenCvImgProc::medianFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::medianBlur(src, dst, 5);
}

void OpenCvImgProc::bilateralFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::bilateralFilter(src, dst, 9, 250, 250);
}

void OpenCvImgProc::lowPassFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::blur(src, dst, cv::Size(5,5));
}

void OpenCvImgProc::lowPassFilterFreq(const cv::Mat& src, cv::Mat& dst) const {
    cv::blur(src, dst, cv::Size(5,5));
}

void OpenCvImgProc::highPassFilter(const cv::Mat& src, cv::Mat& dst, FilterMode mode) const {
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        cv::Mat kernel{-1, 0, 1, -2, 0, 2, -1, 0, 1};
        cv::filter2D(src, dst, -1, kernel);
    };

    if(mode == MULTI_CHANNEL){
        filter(src, dst);
        return;
    }

    this->rgbFilter(src, dst, filter);
    return;
}

void OpenCvImgProc::highPassFilterFreq(const cv::Mat& src, cv::Mat& dst) const {
    dst = src.clone();
    return;
}

void OpenCvImgProc::histEqualize(const cv::Mat& src, cv::Mat& dst) const {
    histogramEqualization(src, dst);
}

void OpenCvImgProc::rgbFilter(const cv::Mat& src, cv::Mat& dst, std::function<void(const cv::Mat&, cv::Mat&)> filter) const{
    cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
    cv::Mat HSVChannels[3];
    cv::split(dst, HSVChannels);

    filter(src, dst);

    std::vector<cv::Mat> channels = {HSVChannels[0],HSVChannels[1],HSVChannels[2]};
    cv::merge(channels,dst);
    cv::cvtColor(dst,dst, cv::COLOR_HSV2BGR);
}
