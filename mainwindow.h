#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "opencv2/core/core.hpp"
#include <functional>
#include "ImgStore.h"
#include "ImgProc.h"
#include "histogramEqualization.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef std::function<void(const cv::Mat&, cv::Mat&)>  filterFunction;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow() noexcept;

    private:
        enum mode {
            GreyScale,
            RGB
        };
        enum theme {
            dark,
            light
        };

        ImgStore& store;
        ImgProc* imgProc;
        std::string currentFileName = "";
        Ui::MainWindow *ui;
        std::vector<filterFunction> filterFunctions = {
            [=](const cv::Mat& src, cv::Mat& dst) {
                this->imgProc->gaussianFilter(src, dst);
            },
            [=](const cv::Mat& src, cv::Mat& dst) {
                    this->imgProc->medianFilter(src, dst);
            },
            [=](const cv::Mat& src, cv::Mat& dst) {
                    this->imgProc->lowPassFilter(src, dst);
            },
            [=](const cv::Mat& src, cv::Mat& dst) {
                    this->imgProc->highPassFilter(src, dst);
            },
            [=](const cv::Mat& src, cv::Mat& dst) {
                    this->imgProc->histEqualize(src, dst);
            }
        };


        void resizeEvent(QResizeEvent* event);
        void applyFilter(const std::function<void(const cv::Mat&, cv::Mat&)>& filter);
        void displaySpatialandFreq(const cv::Mat& spatialImage, cv::Mat& freqImage);
        void imageLoader(const mode& flag);
        void changeTheme(const theme& themeName);

    private slots:
        void autoUpadateLabelSize();
        void loadImage();
        void loadGreyScaleImage();
        void setLoadedImage(bool loaded, const cv::Mat& image);
        void resetImage();
        void saveFilteredImage();
        void updateFileName(const QString& fileName);
        void showPreview(const cv::Mat& image);
        void deleteCurrentImage();
        void changeFilters();
        void showHist();

        void applyGaussianFilter();
        void applyMedianFilter();
        void applyLowPassFilter();
        void applyHighPassFilter();
        void applyHistEqualization();


    signals:
        void imageLoaded(bool loaded, const cv::Mat& image);
        void setPreview(const cv::Mat& image);
        
    protected:
    QPoint pressPos;
    bool isMoving{false};
    void mousePressEvent(QMouseEvent* event){
           //save the press position (this is relative to the current widget)
           pressPos= event->pos();
           isMoving= true;
       }
       void mouseMoveEvent(QMouseEvent* event){
           //isMoving flag makes sure that the drag and drop event originated
           //from within the titlebar, because otherwise the window shouldn't be moved
           if(isMoving){
               //calculate difference between the press position and the new Mouse position
               //(this is relative to the current widget)
               QPoint diff= event->pos() - pressPos;
               //move the window by diff
               window()->move(window()->pos()+diff);
           }
       }
       void mouseReleaseEvent(QMouseEvent* /*event*/){
           //drag and drop operation end
           isMoving= false;
       }
};

#endif // MAINWINDOW_H
