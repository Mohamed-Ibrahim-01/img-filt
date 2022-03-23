#include "ImgStore.h"
#include <iostream>

#ifndef updatedImage
#define updatedImage first
#define originalImage second
#endif

ImgStore::ImgStore(){
    _imgStore = std::unordered_map<std::string, QPair<cv::Mat,cv::Mat>>();
}

ImgStore& ImgStore::get(){
    static ImgStore _store;
    return _store;
}


void ImgStore::addImage(const std::string& imageName, const cv::Mat& img){
    _imgStore[imageName] = qMakePair(img,img.clone());
}

cv::Mat& ImgStore::getImage(const std::string& imageName){
    return _imgStore[imageName].updatedImage;
}

cv::Mat& ImgStore::getOriginalImage(const std::string& imageName){
    return _imgStore[imageName].originalImage;
}

void ImgStore::updateImage(const std::string& imageName, const cv::Mat& img){
    _imgStore[imageName].updatedImage = img;
}

bool ImgStore::check_Key(const std::string& imageName){
    return (_imgStore.find(imageName) != _imgStore.end());
}

void ImgStore::deleteImage(const std::string& imageName){
    _imgStore.erase(imageName);
}

