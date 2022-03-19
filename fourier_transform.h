#ifndef FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_H
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

Mat convertToFourier(Mat I);
Mat adjustSize(Mat I);
Mat constructComplexNumbers(Mat planes[]);
#endif // FOURIER_TRANSFORM_H
