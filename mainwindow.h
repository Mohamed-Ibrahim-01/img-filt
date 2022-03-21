#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "opencv2/core/core.hpp"
#include <functional>
#include "ImgStore.h"
#include "ImgProc.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        ImgStore& store;
        ImgProc* imgProc;
        std::string currentFileName = "";
        Ui::MainWindow *ui;

        void resizeEvent(QResizeEvent* event);
        void applyFilter(std::function<void(const cv::Mat&, cv::Mat&)> filter);

    private slots:
        void autoUpadateLabelSize();
        void loadImage();
        void setLoadedImage(bool loaded, std::string imageName);

        void applyGaussianFilter();
        void applyMedianFilter();
        void applyAverageFilter();
        void applyBilateralFilter();
        void applyHistEqualization();


    signals:
        void imageLoaded(bool loaded, std::string imageName);
};

#endif // MAINWINDOW_H
