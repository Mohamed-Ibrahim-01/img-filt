#pragma once

#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include "QTCV.h"

#define  store ImgStore::get()

class ImgStore{
    private:
        std::unordered_map<std::string, cv::Mat> _img_store;
        ImgStore();

    public:
        cv::Mat get_image(std::string img_name);
        void add_image(std::string img_name, cv::Mat const & img);

        ImgStore(ImgStore const &) = delete;
        ImgStore& operator=(ImgStore const &) = delete;

        static ImgStore& get();

};
