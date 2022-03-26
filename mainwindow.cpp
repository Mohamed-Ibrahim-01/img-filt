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
    this->changeTheme(light);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::loadImage);
    connect(ui->actionOpen_File_as_greyscale, &QAction::triggered, this, &MainWindow::loadGreyScaleImage);
    connect(this, &MainWindow::imageLoaded, this, &MainWindow::setLoadedImage);

    connect(ui->filter_1_btn, &QPushButton::released, this, &MainWindow::applyGaussianFilter);
    connect(ui->filter_2_btn, &QPushButton::released, this, &MainWindow::applyMedianFilter);
    connect(ui->filter_3_btn, &QPushButton::released, this, &MainWindow::applyLowPassFilter);
    connect(ui->filter_4_btn, &QPushButton::released, this, &MainWindow::applyHighPassFilter);
    connect(ui->filter_5_btn, &QPushButton::released, this, &MainWindow::applyHistEqualization);

    connect(ui->resetBtn, &QPushButton::released, this, &MainWindow::resetImage);
    connect(ui->applyBtn, &QPushButton::released, this, &MainWindow::saveFilteredImage);
    connect(ui->deleteBtn, &QPushButton::released, this, &MainWindow::deleteCurrentImage);
    connect(ui->imageNamesComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateFileName);

    connect(this, &MainWindow::setPreview, this, &MainWindow::showPreview);

    connect(ui->spatialFiltersRadioBtn, &QRadioButton::toggled, this, &MainWindow::changeFilters);


    connect(ui->actiondark, &QAction::triggered, [=](...){
       this->changeTheme(dark);
    });
    connect(ui->actionlight, &QAction::triggered, [=](...){
       this->changeTheme(light);
    });

}

MainWindow::~MainWindow() noexcept{
    delete ui;
    delete imgProc;
}

void MainWindow::loadGreyScaleImage(){
    imageLoader(GreyScale);
}

void MainWindow::loadImage(){
    imageLoader(RGB);
}

void MainWindow::imageLoader(const mode& flag){
    QString imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    if(imgPath.isEmpty())
        return;
    ui->imageNamesComboBox->addItem(QFileInfo(imgPath).fileName());

    this->currentFileName = QFileInfo(imgPath).fileName().toStdString();

    cv::Mat image = cv::imread(imgPath.toStdString(), flag);

    store.addImage(this->currentFileName, image);
    emit imageLoaded(true, image);
    emit setPreview(image);
}

void MainWindow::setLoadedImage(bool loaded, const cv::Mat& image){
    if(!loaded || image.empty()) return;


    cv::Mat loadedImageGray = image.clone();
    if (image.type() != CV_8UC1) cv::cvtColor(image, loadedImageGray, CV_BGR2GRAY);
    cv::Mat loadedImageGrayFt = convertToFourier(loadedImageGray);
    this->displaySpatialandFreq(image, loadedImageGrayFt);
    int index = ui->imageNamesComboBox->findText(QString::fromStdString(this->currentFileName));
    ui->imageNamesComboBox->setCurrentIndex(index);
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
    this->setMinimumHeight(height+240);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
   QMainWindow::resizeEvent(event);
   this->autoUpadateLabelSize();
}

void MainWindow::applyFilter(const std::function<void(const cv::Mat&, cv::Mat&)>& filter){
    if (this->currentFileName == "") return;

    cv::Mat& image = store.getImage(this->currentFileName);
    cv::Mat filtered;

    filter(image, filtered);
    cv::Mat imageGray = filtered.clone();
    if (filtered.type() != CV_8UC1) cv::cvtColor(filtered, imageGray, CV_BGR2GRAY);
    cv::Mat loadedImageGrayFt = convertToFourier(imageGray);
    this->displaySpatialandFreq(filtered, loadedImageGrayFt);
}

void MainWindow::displaySpatialandFreq(const cv::Mat& spatialImage, cv::Mat& freqImage){
    QPixmap filteredImagePix = QTCV::mat2QPixmap(spatialImage);
    QPixmap filteredImageFreqPix = QTCV::mat2QPixmap(freqImage);
    ui->shownPic->setPixmap(filteredImagePix);
    ui->shownFreqPic->setPixmap(filteredImageFreqPix);
    this->autoUpadateLabelSize();
}

void MainWindow::applyGaussianFilter(){
    this->applyFilter(filterFunctions[0]);
}

void MainWindow::applyMedianFilter(){
    this->applyFilter(filterFunctions[1]);
}

void MainWindow::applyLowPassFilter(){
    this->applyFilter(filterFunctions[2]);
}

void MainWindow::applyHighPassFilter(){
    this->applyFilter(filterFunctions[3]);
}

void MainWindow::applyHistEqualization(){
    this->applyFilter(filterFunctions[4]);
}

void MainWindow::resetImage(){
    const cv::Mat& image = store.getOriginalImage(this->currentFileName);
    store.updateImage(this->currentFileName,image);
    emit imageLoaded(true, image);
    emit setPreview(image);
}

void MainWindow::saveFilteredImage(){
    QPixmap pixImage = ui->shownPic->pixmap(Qt::ReturnByValue);
    cv::Mat image = QTCV::QPixmap2Mat(pixImage);
    store.updateImage(this->currentFileName,image);
    emit setPreview(image);
}

void MainWindow::updateFileName(const QString& fileName){
    this->currentFileName = fileName.toStdString();
    const cv::Mat& image = store.getImage(this->currentFileName);
    emit imageLoaded(true, image);
    emit setPreview(image);
}


void MainWindow::showPreview(const cv::Mat& image){

    QLabel* previewPanels[] = {
        ui->preview_filter_1,
        ui->preview_filter_2,
        ui->preview_filter_3,
        ui->preview_filter_4,
        ui->preview_filter_5
    };

    if (image.empty()){
        for (const auto& previewPanel: previewPanels)
            previewPanel->clear();
        ui->shownPic->clear();
        ui->shownFreqPic->clear();
        return;
    }

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(image.cols/4,image.rows/4));

    int width = lround(double(image.cols)/image.rows * ui->preview_filter_1->height());
    for (size_t counter = 0; counter < 5; counter++){
        previewPanels[counter]->setFixedWidth(width);
        cv::Mat filteredResizedImage;
        filterFunctions[counter](resizedImage,filteredResizedImage);
        previewPanels[counter]->setPixmap(QTCV::mat2QPixmap(filteredResizedImage));
    }
}

void MainWindow::deleteCurrentImage(){
    const QString& currenImageName = ui->imageNamesComboBox->currentText();
    store.deleteImage(currenImageName.toStdString());
    ui->imageNamesComboBox->removeItem(ui->imageNamesComboBox->currentIndex());
    if (ui->imageNamesComboBox->count() > 0){
        ui->imageNamesComboBox->setCurrentIndex(0);
        const QString& newImageName = ui->imageNamesComboBox->itemText(0);
        const cv::Mat& image = store.getImage(newImageName.toStdString());
        this->currentFileName = newImageName.toStdString();
        emit imageLoaded(true, image);
        emit setPreview(image);
    } else {
        cv::Mat emptyImage;
        emit setPreview(emptyImage);
    }

}

void MainWindow::changeTheme(const theme &themeName){
    bool theme = themeName == dark;
    ui->actiondark->setChecked(theme);
    ui->actionlight->setChecked(!theme);
    QString str = theme ? ":qdarkstyle/dark/darkStyle.qss":":qdarkstyle/light/lightStyle.qss";
    QFile themeFile(str);
    themeFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeText(&themeFile);
    this->setStyleSheet(themeText.readAll());
    themeFile.close();
}

void MainWindow::changeFilters(){
    bool freqFilters = ui->freqFiltersRadioBtn->isChecked();
    if (freqFilters){
        this->filterFunctions[2] = [=] (const cv::Mat& src, cv::Mat& dst) {
            this->imgProc->lowPassFilterFreq(src, dst);
        };
        this->filterFunctions[3] = [=] (const cv::Mat& src, cv::Mat& dst) {
            this->imgProc->highPassFilterFreq(src, dst);
        };
        QMessageBox messageBox;
        messageBox.information(0,"Filters","only low and high pass filters will be in the frequancy domain");
        messageBox.setFixedSize(500,200);
    } else {
        this->filterFunctions[2] = [=] (const cv::Mat& src, cv::Mat& dst) {
            this->imgProc->lowPassFilter(src, dst);
        };
        this->filterFunctions[3] = [=] (const cv::Mat& src, cv::Mat& dst) {
            this->imgProc->highPassFilter(src, dst);
        };
    }
    emit setPreview(store.getImage(this->currentFileName));
}
