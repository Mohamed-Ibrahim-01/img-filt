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
namespace fs = std::filesystem;

using namespace cv;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::loadImage);
}

MainWindow::~MainWindow() {
    delete ui;
}

cv::Mat MainWindow::loadImage(){
    QString imgPath = QFileDialog::getOpenFileName(this, "Open a file", "", "Images (*.png *.xpm *.jpg *.bmb)");
    cv::Mat image = cv::imread(imgPath.toStdString(), 1);
    return image;
}
