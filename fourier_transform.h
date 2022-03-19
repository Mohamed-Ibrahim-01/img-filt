#ifndef FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_H
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <array>
#include <iostream>
using namespace std;
using namespace cv;

Mat convertToFourier(Mat I);
Mat adjustSize(Mat I);
Mat constructComplexNumbers(Mat planes[]);
Mat prepareMagnitude(Mat planes[]);
array<Mat,4> makeQuarters(Mat magI);
array<Mat,4> reArrangeQuarters(array<Mat,4> quarters);
#endif // FOURIER_TRANSFORM_H
