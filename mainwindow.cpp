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

    std::string imageName = QFileInfo(imgPath).fileName().toStdString();
    cv::Mat image = cv::imread(imgPath.toStdString(), 1);
    store.addImage(imageName, image);
    emit imageLoaded(true, imageName);
}

void MainWindow::setLoadedImage(bool loaded, std::string imageName){

    if(loaded){
        QPixmap lenna = QTCV::mat2QPixmap(store.getImage(imageName));
        ui->shownPic->setPixmap(lenna);
    }
}
