#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    // Low Pass Filter With Kernel
//    float sum;
////    loading the image and reading it
//    QString img_path = QFileDialog::getOpenFileName(this, "Open a file", "", "Images (*.png *.xpm *.jpg *.bmb)");
//    int fixed_height = 500;
//    int fixed_width = 600;
//    cv::Mat image = cv::imread(img_path.toStdString(), 1);
//    cv::Mat resized_img;
//    cv::resize(image, resized_img,cv::Size(fixed_width, fixed_height));
//    float Kernel[4][4] = {
//                                {1/9.0, 1/9.0, 1/9.0, 1/9.0},
//                                {1/9.0, 1/9.0, 1/9.0, 1/9.0},
//                                {1/9.0, 1/9.0, 1/9.0, 1/9.0},
//              {1/9.0, 1/9.0, 1/9.0, 1/9.0}
//                               };

//   cv::Mat dst = resized_img.clone();
//   for(int y = 0; y < resized_img.rows; y++)
//       for(int x = 0; x < resized_img.cols; x++)
//           dst.at<uchar>(y,x) = 0.0;
//   //convolution operation
//   for(int y = 1; y < resized_img.rows - 1; y++){
//       for(int x = 1; x < resized_img.cols - 1; x++){
//           sum = 0.0;
//           for(int k = -1; k <= 1;k++){
//               for(int j = -1; j <=1; j++){
//                   sum = sum + Kernel[j+1][k+1]*resized_img.at<uchar>(y - j, x - k);
//               }
//           }
//           dst.at<uchar>(y,x) = sum;
//       }
//   }

//   cv::namedWindow("final");
//   cv::imshow("final", dst);

//   cv::namedWindow("initial");
//   cv::imshow("initial", resized_img);

//     Loading the image to our work space
     QString img_path = QFileDialog::getOpenFileName(this, "Open a file", "", "Images (*.png *.xpm *.jpg *.bmb)");
     int fixed_height = 300;
     int fixed_width = 500;
     cv::Mat image = cv::imread(img_path.toStdString(), 1);
     cv::Mat resized_img;
     cv::resize(image, resized_img,cv::Size(fixed_width,fixed_height));
     QImage temp = QImage((unsigned char*) resized_img.data,resized_img.cols,resized_img.rows, static_cast<int>(resized_img.step), QImage::Format_RGB888);
     temp = temp.rgbSwapped();
     QPixmap testPixmap = QPixmap::fromImage(temp);
     ui->label->setPixmap(testPixmap);
//      Appling Filters Median Filter
     cv::Mat median_filter_image;
     cv::cvtColor(resized_img, median_filter_image, cv::COLOR_BGR2GRAY);
     cv::medianBlur(median_filter_image, median_filter_image, 5);
     cv::namedWindow("Median Filter");
     cv::imshow("Median Filter", median_filter_image);

//     cv::Scalar intensity1=0;
//     cv::Mat src1,src2;

//     src2 = image.clone();

//     //If image not found
//     if (!image.data)
//         {
//          std::cout << "No image data \n";

//         }

//     //Take the mask size
//     //It should be less than that of size of the image
//      std::cout<<"Enter the mask Size =";
//      a = 2;

//     //for loop for counting the number of rows and columns and displaying the pixel value at each point
//       for (int i = 0; i < src1.rows-a; i++)
//       {
//        for (int j = 0; j < src1.cols-a; j++)
//         {
//       cv::Scalar intensity2;
//       for (int p = 0; p<a; p++)
//           {

//           for (int q = 0; q<a; q++)
//            {

//          intensity1 = src1.at<uchar>(i+p,j+q);
//          intensity2.val[0] +=intensity1.val[0];
//         }
//      }
//        src2.at<uchar>(i+(a-1)/2,j+(a-1)/2)=intensity2.val[0]/(a*a);
//       }
//        }

//       //Display the original image
//        cv::namedWindow("Display Image");
//        imshow("Display Image", src1);

//       //Display the Low Pass Filtered Image image
//        cv::namedWindow("Low Pass Filtered Image");
//         imshow("Low Pass Filtered Image", src2);




}


