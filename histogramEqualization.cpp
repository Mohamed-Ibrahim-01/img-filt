#include "histogramEqualization.h"
/** histogram equalization using the following equation
 *
 *  new equalized value =  Round((CDF(i) - CDF min)/(Width * height - CDF min) * (number of possible colors - 1))
 *
 *  cause we need the colors from 0 - 255 so subtract one from the number of possible colors
 *
 *  the CDF min is the first non zero value of greyscale vaules
 *
 *  ref => https://en.wikipedia.org/wiki/Histogram_equalization#Small_image
 */

#include <vector>
#include <unordered_map>
#include <cmath>


void countIntensity(QVector<unsigned long>& greyScaleCounter, const cv::Mat& src){
    for (int rowCounter = 0; rowCounter < src.rows; rowCounter++){
        for (int colCounter = 0; colCounter < src.cols; colCounter++){

            const unsigned char& pixelVal = src.at<unsigned char>(rowCounter,colCounter);

            greyScaleCounter[pixelVal] += 1;
        }
    }
}

void countIntensityWithCDF(std::vector<unsigned long>& greyScaleCounter, const cv::Mat& src, unsigned char& CDFminIndex){
    for (int rowCounter = 0; rowCounter < src.rows; rowCounter++){
        for (int colCounter = 0; colCounter < src.cols; colCounter++){

            const unsigned char& pixelVal = src.at<unsigned char>(rowCounter,colCounter);

            // getting the lowest pixel value index as it will be the first non zero value so we can know the CDF min after
            // finishing scanning the image
            if (pixelVal < CDFminIndex) CDFminIndex = pixelVal;

            greyScaleCounter[pixelVal] += 1;
        }
    }
}

void equalization(const cv::Mat& src, cv::Mat& dst){

    // check if the input array is 1 channel and throw an error if not
    if (src.type() != CV_8UC1){
        throw "equalization(const cv::Mat& src, cv::Mat& dst) -> unsupported operation";
    }

    // the number of possible colors is 256 as it is 8 bit channel
    std::vector<unsigned long> greyScaleCounter(256,0);
    unsigned char CDFminIndex = 255;
    std::unordered_map<int,int> greyScaleMap;

    countIntensityWithCDF(greyScaleCounter, src, CDFminIndex);


    // calculating the constent part of the equation (number of possible colors - 1)/(Width * height - CDF min)
    double changingCoff = double(255)/(src.rows*src.cols - greyScaleCounter[CDFminIndex]);

    // calculting the new values using the histogram equalization equation and then mapping the old values to the new values
    for (int counter = CDFminIndex; counter < 256; counter++){

        if (counter == 0) continue;

        greyScaleCounter[counter] += greyScaleCounter[counter - 1];

        // check if the value is already mapped before and if so then skip the calculation and check other pixel values
        if (greyScaleMap.find(counter) != greyScaleMap.end()) continue;

        greyScaleMap[counter] = lround(changingCoff * (greyScaleCounter[counter] - greyScaleCounter[CDFminIndex]));
    }

    // scanning the output image pixel by pixel to update the old values with the new values
    for (int rowCounter = 0; rowCounter < src.rows; rowCounter++){
        for (int colCounter = 0; colCounter < src.cols; colCounter++){
            unsigned char& pixelVal = dst.at<unsigned char>(rowCounter,colCounter);
            pixelVal = greyScaleMap[pixelVal];
        }
    }

}


// to equalise RGB image we need to convert it to HSV color space and then equilse the V values then convert it back to the RGB color space
void RGBEqualization(const cv::Mat& src, cv::Mat& dst){
    cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
    cv::Mat HSVChannels[3];
    cv::split(dst, HSVChannels);
    equalization(HSVChannels[2],HSVChannels[2]);
    std::vector<cv::Mat> channels = {HSVChannels[0],HSVChannels[1],HSVChannels[2]};
    cv::merge(channels,dst);
    cv::cvtColor(dst,dst, cv::COLOR_HSV2BGR);
}

// we only need to send the image as it is a single channel image
inline void greyScaleEqualization(const cv::Mat& src, cv::Mat& dst){
    equalization(src,dst);
}

// the main function as it checks first for the type of the image then call the proper funtion for it
void histogramEqualization(const cv::Mat& src, cv::Mat& dst){

    // cloning the input image to the output image to make sure they have the same pixel values and size so we can update the old pixel
    // value with the new value by mapping them in the equalization function
    dst = src.clone();

    switch (src.type()) {
        // 3 channels, 8bits
        case CV_8UC3:
            RGBEqualization(src,dst);
            break;
        // 1 channel, 8bits
        case CV_8UC1:
            greyScaleEqualization(src,dst);
            break;
    }
}
