#ifndef IMGSTORE_H
#define IMGSTORE_H

#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include "QTCV.h"


class ImgStore{
    private:
        std::unordered_map<std::string, cv::Mat> _imgStore;
        ImgStore();

    public:
        cv::Mat getImage(std::string imageName);
        void addImage(std::string imageName, cv::Mat const & img);

        ImgStore(ImgStore const &) = delete;
        ImgStore& operator=(ImgStore const &) = delete;

        static ImgStore& get();

};

#endif
