#pragma once

#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include "QTCV.h"

#define  store ImgStore::get()

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
