#include "QTCV.h"
/** big endian systems vs little endian systems
 *
 * from the qt docs some of the QImage's formats are endian dependent
 *
 * in little endian systems:
 *  QImage::Format_ARGB32:     B G R A
 *  QImage::Format_RGB888:     R G B
 * and in the big endian systems:
 *  QImage::Format_ARGB32:     A R G B
 *  QImage::Format_RGB888:     R G B
 *
 *  notice that the RGB888 color format is endian independent so we don't have to check we will just swap the colors of it to suit cv::Mat
 *  but we need to check for the system type for the ARGB32 before deciding to swap or not
 */

QImage QTCV::mat2QImage(const cv::Mat& image){

    // 2 flags to specifiy the the format of the image and if the color swapped from RGB to BGR as it is the default setting for cv::Mat
    QImage::Format flag;
    bool swapColors = false;

    switch(image.type()){
        // 4 channels,  8 bits
        case CV_8UC4:
            flag = QImage::Format_ARGB32;

            // to check if the system use the big endian or little endian as if it is a big endian then we will need to swap colors in 4 channels mode
            #if Q_BYTE_ORDER == Q_BIG_ENDIAN
            swapColors = true;
            #endif

            break;
        // 3 channels, 8 bits
        case CV_8UC3:
            flag = QImage::Format_RGB888;
            swapColors = true;
            break;
        // 1 channel, 8bits
        case CV_8UC1:
            flag = QImage::Format_Grayscale8;
            break;
        default:
            throw std::invalid_argument("QTCV::mat2QImage -> unsupported cv::Mat type");
            break;

    }

    QImage convertedImage(image.data,
                          image.cols,image.rows,
                          static_cast<int>(image.step),
                          flag);

    convertedImage = swapColors ? convertedImage.rgbSwapped() : convertedImage;
    return convertedImage;
}

QPixmap QTCV::mat2QPixmap(const cv::Mat& image){
    return QPixmap::fromImage(mat2QImage(image));
}

cv::Mat QTCV::QImage2Mat(const QImage& image){
    // 2 flags to specifiy the the format of the image and if the color swapped from RGB to BGR as it is the default setting for cv::Mat
    int flag;
    bool swapColors = false;

    switch (image.format()) {
        // 4 channels, 8 bits
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
            flag = CV_8UC4;

            // to check if the system use the big endian or little endian as if it is a big endian then we will need to swap colors in 4 channels mode
            #if Q_BYTE_ORDER == Q_BIG_ENDIAN
            swapColors = true;
            #endif

            break;
        // 3 channels, 8 bits
        case QImage::Format_RGB888:
            flag =  CV_8UC3;
            swapColors = true;
            break;
        // 1 channel, 8 bits
        case QImage::Format_Grayscale8:
        case QImage::Format_Indexed8:
            flag = CV_8UC1;
            break;
        default:
            throw std::invalid_argument("QTCV::QImage2Mat -> unsupported QImage format");
            break;
        // a special case as the RGB32 image type takes a space of 4 channels 8bit image so we need to convert it ARGB first then
        // then convert the ARGB to RGB image
        case QImage::Format_RGB32:
            cv::Mat  mat( image.height(), image.width(),
                                      CV_8UC4,
                                      const_cast<uchar*>(image.bits()),
                                      static_cast<size_t>(image.bytesPerLine())
                                      );
            cv::Mat matnoAlpha;
            cv::cvtColor(mat,matnoAlpha,cv::COLOR_BGRA2BGR); // drop the alpha
            return matnoAlpha;

    }

    auto convertableImage = swapColors ? image.rgbSwapped() : image;

    return cv::Mat(convertableImage.height(),convertableImage.width(),
                   flag,
                   const_cast<uchar*>(convertableImage.bits()),
                   static_cast<size_t>(convertableImage.bytesPerLine())
                   ).clone();
}

cv::Mat QTCV::QPixmap2Mat(const QPixmap& image){
    return QImage2Mat(image.toImage());
}
