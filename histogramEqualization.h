#ifndef HISTOGRAMEQUALIZATION_H
#define HISTOGRAMEQUALIZATION_H

#include <opencv2/imgproc.hpp>
#include "histogramwindow.h"

void histogramEqualization(const cv::Mat& src, cv::Mat& dst);

void countIntensity(QVector<unsigned long>& greyScaleCounter, const cv::Mat& src);

#endif // HISTOGRAMEQUALIZATION_H
