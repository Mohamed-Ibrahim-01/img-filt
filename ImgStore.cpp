#include "ImgStore.h"
#include <iostream>

ImgStore::ImgStore(){
    _img_store = std::unordered_map<std::string, cv::Mat>();
}

ImgStore& ImgStore::get(){
    static ImgStore _store;
    return _store;
}


void ImgStore::add_image(std::string img_name, cv::Mat const & img){
    _img_store.insert(std::make_pair(img_name, img));
}

cv::Mat ImgStore::get_image(std::string img_name){
    return _img_store[img_name];
}
