#include "ImgStore.h"
#include <iostream>

ImgStore::ImgStore(){
    _imgStore = std::unordered_map<std::string, cv::Mat>();
}

ImgStore& ImgStore::get(){
    static ImgStore _store;
    return _store;
}


void ImgStore::addImage(std::string imageName, cv::Mat const & img){
    _imgStore.insert(std::make_pair(imageName, img));
}

cv::Mat ImgStore::getImage(std::string imageName){
    return _imgStore[imageName];
}
