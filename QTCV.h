#ifndef QTCV_H
#define QTCV_H

#include <QtWidgets>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

namespace QTCV {

    QImage mat2QImage(const cv::Mat& image);

    QPixmap mat2QPixmap(const cv::Mat& image);

    cv::Mat QImage2Mat(const QImage& image);

    cv::Mat QPixmap2Mat(const QPixmap& image);

}

#endif // QTCV_H
