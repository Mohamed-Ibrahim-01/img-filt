#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

#include <iostream>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include "fourier_transform.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) ,store(ImgStore::get()),
    imgProc(new OpenCvImgProc), ui(new Ui::MainWindow){

    ui->setupUi(this);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::loadImage);
    connect(this, &MainWindow::imageLoaded, this, &MainWindow::setLoadedImage);

    connect(ui->filter_1_btn, &QPushButton::released, this, &MainWindow::applyGaussianFilter);
    connect(ui->filter_2_btn, &QPushButton::released, this, &MainWindow::applyMedianFilter);
    connect(ui->filter_3_btn, &QPushButton::released, this, &MainWindow::applyLowPassFilter);
    connect(ui->filter_4_btn, &QPushButton::released, this, &MainWindow::applyHighPassFilter);
    connect(ui->filter_5_btn, &QPushButton::released, this, &MainWindow::applyHistEqualization);
}

MainWindow::~MainWindow() {
    delete ui;
    delete imgProc;
}

void MainWindow::loadImage(){
    QString imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    if(imgPath.isEmpty()){
        emit imageLoaded(false, "no image");
        return;
    }

    this->currentFileName = QFileInfo(imgPath).fileName().toStdString();
    cv::Mat image = cv::imread(imgPath.toStdString(), 1);
    store.addImage(this->currentFileName, image);
    emit imageLoaded(true, this->currentFileName);
}

void MainWindow::setLoadedImage(bool loaded, std::string imageName){
    if(loaded){
        cv::Mat loadedImage = store.getImage(imageName), loadedImageGray;
        cv::cvtColor(loadedImage, loadedImageGray, CV_BGR2GRAY);
        cv::Mat loadedImageGrayFt = convertToFourier(loadedImageGray);
        this->displaySpatialandFreq(loadedImage, loadedImageGrayFt);
    }
}

void MainWindow::autoUpadateLabelSize(){
    
    // check if there is fileloaded if not then return
    if (this->currentFileName == "") return;

    // get the image stored based on the file name
    const cv::Mat& image = store.getImage(this->currentFileName);

    // calculating the ratio of the image and then calculate the new height based on the current width of the label
    int height = lround(double(image.rows)/image.cols * ui->shownPic->width());

    // setting the height of the shown pic and the shown freq pic to maintain the aspect ratio of the image
    ui->shownPic->setFixedHeight(height);
    ui->shownFreqPic->setFixedHeight(height);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
   QMainWindow::resizeEvent(event);
   this->autoUpadateLabelSize();
}

void MainWindow::applyFilter(std::function<void(const cv::Mat&, cv::Mat&)> filter){
    if (this->currentFileName == "") return;

    cv::Mat& image = store.getImage(this->currentFileName);
    cv::Mat filtered, imageGray;

    filter(image, filtered);

    cv::cvtColor(filtered, imageGray, CV_BGR2GRAY);
    cv::Mat loadedImageGrayFt = convertToFourier(imageGray);
    this->displaySpatialandFreq(filtered, loadedImageGrayFt);
}

void MainWindow::displaySpatialandFreq(cv::Mat& spatialImage, cv::Mat& freqImage){
    QPixmap filteredImagePix = QTCV::mat2QPixmap(spatialImage);
    QPixmap filteredImageFreqPix = QTCV::mat2QPixmap(freqImage);
    ui->shownPic->setPixmap(filteredImagePix);
    ui->shownFreqPic->setPixmap(filteredImageFreqPix);
    this->autoUpadateLabelSize();
}

void MainWindow::applyGaussianFilter(){
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        this->imgProc->gaussianFilter(src, dst);
    };
    this->applyFilter(filter);
}

void MainWindow::applyMedianFilter(){
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        this->imgProc->medianFilter(src, dst);
    };
    this->applyFilter(filter);
}

void MainWindow::applyLowPassFilter(){
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        this->imgProc->lowPassFilter(src, dst);
    };
    this->applyFilter(filter);
}

void MainWindow::applyHighPassFilter(){
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        auto mode = imgProc->V_HSV;
        this->imgProc->highPassFilter(src, dst, mode);
    };
    this->applyFilter(filter);
}

void MainWindow::applyHistEqualization(){
    std::function<void(const cv::Mat&, cv::Mat&)> filter = [=](const cv::Mat& src, cv::Mat& dst) {
        this->imgProc->histEqualize(src, dst);
    };
    this->applyFilter(filter);

}
