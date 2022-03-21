#ifndef IMGPROC_H
#define IMGPROC_H
#include <opencv2/imgproc.hpp>


class ImgProc {
    public:
        virtual ~ImgProc() {}
        virtual void gaussianFilter(const cv::Mat& src, cv::Mat& dst) const = 0;
        virtual void medianFilter(const cv::Mat& src, cv::Mat& dst) const = 0;
        virtual void averageFilter(const cv::Mat& src, cv::Mat& dst) const = 0;
        virtual void bilateralFilter(const cv::Mat& src, cv::Mat& dst) const = 0;
        virtual void histEqualize(const cv::Mat& src, cv::Mat& dst) const = 0;
};


/* This is not used for now (It should be used if we needed to support runtime implementation change)*/
class ImgProcImplementation {
    private:
        ImgProc *_imgProc;

    public:
        ImgProcImplementation(ImgProc *imgProc = nullptr) : _imgProc(imgProc) { }
        ~ImgProcImplementation() { delete this->_imgProc; }

        void setImplementation(ImgProc *imgProc) {
            delete this->_imgProc;
            this->_imgProc = imgProc;
        }

        void gaussianFilter(const cv::Mat& src, cv::Mat& dst) const;
        void medianFilter(const cv::Mat& src, cv::Mat& dst) const;
        void averageFilter(const cv::Mat& src, cv::Mat& dst) const;
        void bilateralFilter(const cv::Mat& src, cv::Mat& dst) const;
};


/* This is not used for now (This is a skeleton implementation that do nothing)*/
class FromScratchImgProc : public ImgProc {
    public:
        void gaussianFilter(const cv::Mat& src, cv::Mat& dst) const override { dst = src; }
        void medianFilter(const cv::Mat& src, cv::Mat& dst) const override { dst = src; }
        void averageFilter(const cv::Mat& src, cv::Mat& dst) const override { dst = src; }
        void bilateralFilter(const cv::Mat& src, cv::Mat& dst) const override { dst = src; }
};


class OpenCvImgProc : public ImgProc {
    public:
        void gaussianFilter(const cv::Mat& src, cv::Mat& dst) const override;
        void medianFilter(const cv::Mat& src, cv::Mat& dst) const override;
        void averageFilter(const cv::Mat& src, cv::Mat& dst) const override;
        void bilateralFilter(const cv::Mat& src, cv::Mat& dst) const override;
        void histEqualize(const cv::Mat& src, cv::Mat& dst) const override;
};

#endif // IMGPROC_H
