#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

#include <iostream>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <filesystem>
#include "ImgStore.h"
namespace fs = std::filesystem;

using namespace cv;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , store(ImgStore::get()), ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::loadImage);
    connect(this, &MainWindow::imageLoaded, this, &MainWindow::setLoadedImage);
}

MainWindow::~MainWindow() {
    delete ui;
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
        QPixmap lenna = QTCV::mat2QPixmap(store.getImage(imageName));
        ui->shownPic->setPixmap(lenna);
        ui->shownFreqPic->setPixmap(lenna);
        this->autoUpadateLabelSize();
    }
}

void MainWindow::autoUpadateLabelSize(){
    
    // check if there is fileloaded if not then return
    if (this->currentFileName == "") return;

    // get the image stored based on the file name
    ImgStore& store = ImgStore::get();
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
