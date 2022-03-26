#include "ImgProc.h"
#include "fourier_transform.h"
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
    if (src.channels() > 1) {
        cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
        cv::Mat HSVChannels[3];
        cv::split(dst, HSVChannels);
        cv::Mat mask = lowPassMask(getfourierPaddedSize(HSVChannels[2]));
        freqFilter(HSVChannels[2], HSVChannels[2], mask);
        std::vector<cv::Mat> channels = {HSVChannels[0],HSVChannels[1],HSVChannels[2]};
        cv::merge(channels,dst);
        cv::cvtColor(dst,dst, cv::COLOR_HSV2BGR);
        return;
    }
    cv::Mat mask = lowPassMask(getfourierPaddedSize(src));
    freqFilter(src, dst, mask);
}

void OpenCvImgProc::highPassFilter(const cv::Mat& src, cv::Mat& dst) const {
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        cv::Mat kernel{-1, 0, 1, -2, 0, 2, -1, 0, 1};
        cv::filter2D(src, dst, -1, kernel);
    };

    if(src.channels() > 1){
        this->rgbFilter(src, dst, filter);
        return;
    }

    filter(src, dst);
}

void OpenCvImgProc::highPassFilterFreq(const cv::Mat& src, cv::Mat& dst) const {
    if (src.channels() > 1) {
        cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
        cv::Mat HSVChannels[3];
        cv::split(dst, HSVChannels);
        cv::Mat mask = highPassMask(getfourierPaddedSize(HSVChannels[2]));
        freqFilter(HSVChannels[2], HSVChannels[2], mask);
        std::vector<cv::Mat> channels = {HSVChannels[0],HSVChannels[1],HSVChannels[2]};
        cv::merge(channels,dst);
        cv::cvtColor(dst,dst, cv::COLOR_HSV2BGR);
        return;
    }
    cv::Mat mask = highPassMask(getfourierPaddedSize(src));
    freqFilter(src, dst, mask);
}

void OpenCvImgProc::freqFilter(const cv::Mat& src, cv::Mat& dst, cv::Mat mask) const {
    if (src.channels() > 1){
        throw std::invalid_argument("Mat convertToFourier(Mat img) only works with 1 channel matrix ");
    }

    cv::Mat planes[2];
    cv::Mat realPlane, imaginaryPlane;
    fourierPlanes(src, planes);

    fftShift(planes);
    applyFFtFilter(planes, mask);
    ifftShift(planes);

    cv::merge(planes, 2, dst);
    inverseFourier(dst, dst);

    dst = prepMatForConverting(dst);
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

cv::Mat OpenCvImgProc::highPassMask(cv::Size maskSize) const {
    int cx = maskSize.width/2;
    int cy = maskSize.height/2;
    int rectSize = lround(cx > cy ? cy * 0.6: cx * 0.6);
    int rectTopLeftX = cx-(rectSize/2), rectTopLeftY = cy-(rectSize/2);
    cv::Mat mask = cv::Mat::ones(maskSize, CV_32F);
    mask(cv::Rect(rectTopLeftX, rectTopLeftY, rectSize, rectSize)) = 0;
    return mask;
}

cv::Mat OpenCvImgProc::lowPassMask(cv::Size maskSize) const {
    int cx = maskSize.width/2;
    int cy = maskSize.height/2;
    int rectSize = lround(cx > cy ? cy * 0.35: cx * 0.35);
    int rectTopLeftX = cx-(rectSize/2), rectTopLeftY = cy-(rectSize/2);
    cv::Mat mask = cv::Mat::zeros(maskSize, CV_32F);
    mask(cv::Rect(rectTopLeftX, rectTopLeftY, rectSize, rectSize)) = 1;
    return mask;
}
