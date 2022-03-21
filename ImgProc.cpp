#include "ImgProc.h"
#include "histogramEqualization.h"

void OpenCvImgProc::gaussianFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::GaussianBlur(src, dst, cv::Size(5, 5), 0, 0);
}

void OpenCvImgProc::medianFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::medianBlur(src, dst, 5);
}

void OpenCvImgProc::averageFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::blur(src, dst, cv::Size(5,5));
}

void OpenCvImgProc::bilateralFilter(const cv::Mat& src, cv::Mat& dst) const {
    cv::bilateralFilter(src, dst, 9, 250, 250);
}

void OpenCvImgProc::histEqualize(const cv::Mat& src, cv::Mat& dst) const {
    histogramEqualization(src, dst);
}
