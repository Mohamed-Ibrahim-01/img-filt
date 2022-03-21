#ifndef IMGSTORE_H
#define IMGSTORE_H

#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include <QPair>
#include "QTCV.h"


class ImgStore{
    private:
        std::unordered_map<std::string, QPair<cv::Mat,cv::Mat>> _imgStore;
        ImgStore();

    public:
        cv::Mat& getImage(const std::string& imageName);
        void addImage(const std::string& imageName, const cv::Mat& img);
        cv::Mat& getOriginalImage(const std::string& imageName);
        void updateImage(const std::string& imageName, const cv::Mat& img);

        ImgStore(ImgStore const &) = delete;
        ImgStore& operator=(ImgStore const &) = delete;

        static ImgStore& get();

};

#endif //IMGSTORE_H
