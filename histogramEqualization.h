#ifndef HISTOGRAMEQUALIZATION_H
#define HISTOGRAMEQUALIZATION_H

#include <opencv2/imgproc.hpp>

void histogramEqualization(const cv::Mat& src, cv::Mat& dst);

#endif // HISTOGRAMEQUALIZATION_H
