#ifndef QTCV_H
#define QTCV_H

#include <mainwindow.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

namespace QTCV {
    QImage mat2QImage(const cv::Mat& image){
        QImage::Format flag;
        bool swapColors = false;
        switch(image.type()){
            case CV_8UC4:
                flag = QImage::Format_ARGB32;
                break;
            case CV_8UC3:
                flag = QImage::Format_RGB888;
                swapColors = true;
                break;
            case CV_8UC1:
                flag = QImage::Format_Grayscale8;
                break;
            default:
                throw "QTCV::mat2QImage -> unsupported cv::Mat type";
                break;

        }

        QImage convertedImage(image.data,
                              image.cols,image.rows,
                              static_cast<int>(image.step),
                              flag);

        convertedImage = swapColors ? convertedImage.rgbSwapped() : convertedImage;
        return convertedImage;
    }

    QPixmap mat2QPixmap(const cv::Mat& image){
        return QPixmap::fromImage(mat2QImage(image));
    }

    cv::Mat QImage2Mat(const QImage& image){
        int flag;
        bool swapColors = false;
        switch (image.format()) {
            case QImage::Format_ARGB32:
            case QImage::Format_ARGB32_Premultiplied:
                flag = CV_8UC4;
                break;
            case QImage::Format_RGB888:
                flag =  CV_8UC3;
                swapColors = true;
                break;
            case QImage::Format_Grayscale8:
            case QImage::Format_Indexed8:
                flag = CV_8UC1;
                break;
            default:
                throw "QTCV::QImage2Mat -> unsupported QImage format";
                break;

        }
        auto convertableImage = swapColors ? image.rgbSwapped() : image;

        return cv::Mat(convertableImage.height(),convertableImage.width(),
                       flag,
                       const_cast<uchar*>(convertableImage.bits()),
                       static_cast<size_t>(convertableImage.bytesPerLine())
                       ).clone();
    }

    cv::Mat QPixmap2Mat(const QPixmap& image){
        return QImage2Mat(image.toImage());
    }

}

#endif // QTCV_H
