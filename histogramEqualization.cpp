#include "histogramEqualization.h"

#include <vector>
#include <unordered_map>
#include <cmath>

void equalization(const cv::Mat& src, cv::Mat& dst){
    if (src.type() != CV_8UC1){
        throw "equalization(const cv::Mat& src, cv::Mat& dst) -> unsupported operation";
    }
    std::vector<unsigned int> greyScaleCounter(256,0);
    unsigned char CDFminIndex = 255;
    std::unordered_map<int,int> greyScaleMap;

    for (int rowCounter = 0; rowCounter < src.rows; rowCounter++){
        for (int colCounter = 0; colCounter < src.cols; colCounter++){
            const unsigned char& pixelVal = src.at<unsigned char>(rowCounter,colCounter);
            if (pixelVal < CDFminIndex) CDFminIndex = pixelVal;
            greyScaleCounter[pixelVal] += 1;
        }
    }

    double changingCoff = double(255)/(src.rows*src.cols - greyScaleCounter[CDFminIndex]);

    for (int counter = CDFminIndex; counter < 256; counter++){
        if (counter == 0) continue;
        greyScaleCounter[counter] += greyScaleCounter[counter - 1];
        if (greyScaleMap.find(counter) != greyScaleMap.end()) continue;
        greyScaleMap[counter] = lround(changingCoff * (greyScaleCounter[counter] - greyScaleCounter[CDFminIndex]));
    }

    for (int rowCounter = 0; rowCounter < src.rows; rowCounter++){
        for (int colCounter = 0; colCounter < src.cols; colCounter++){
            unsigned char& pixelVal = dst.at<unsigned char>(rowCounter,colCounter);
            pixelVal = greyScaleMap[pixelVal];
        }
    }

}


void RGBEqualization(const cv::Mat& src, cv::Mat& dst){
    cv::cvtColor(src, dst, CV_BGR2HSV);
    cv::Mat HSVChannels[3];
    cv::split(dst, HSVChannels);
    equalization(HSVChannels[2],HSVChannels[2]);
    std::vector<cv::Mat> channels = {HSVChannels[0],HSVChannels[1],HSVChannels[2]};
    cv::merge(channels,dst);
    cv::cvtColor(dst,dst, CV_HSV2BGR);
}

inline void greyScaleEqualization(const cv::Mat& src, cv::Mat& dst){
    equalization(src,dst);
}

void histogramEqualization(const cv::Mat& src, cv::Mat& dst){
    if (dst.empty()) dst = src.clone();
    switch (src.type()) {
        case CV_8UC3:
            RGBEqualization(src,dst);
            break;
        case CV_8UC1:
            greyScaleEqualization(src,dst);
            break;
    }
}
