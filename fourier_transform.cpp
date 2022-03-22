#include "fourier_transform.h"
#include <stdexcept>

/*
 * function to convert normal image to it's corresponding fourier transform
 * params : CV::Mat object type image
 * return : CV::Mat object type image
 * first we adjust the size of it to be apropriate to fourier
 * by zero padding
 * then construct the complex matrix to apply fourier on it
 * and calculate the magnitude from the resulting matrix
 * with small modification on the magnitude matrix like
 * the logarithmic sclae and rearrangement of it's quarters.
*/
cv::Mat convertToFourier(const cv::Mat& img){
    if (img.channels() > 1){
        throw std::invalid_argument("Mat convertToFourier(Mat img) only works with 1 channel matrix ");
    }
    cv::Mat padded = adjustSize(img);
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexI = constructComplexNumbers(planes);
    dft(complexI, complexI);
    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    cv::Mat magnitude = prepareMagnitude(planes);
    std::array<cv::Mat,4> quarters = makeQuarters(magnitude);
    quarters = reArrangeQuarters(quarters);
    normalize(magnitude, magnitude, 0, 1, cv::NORM_MINMAX);
    magnitude = prepMatForConverting(magnitude);
    return magnitude;
}
/*
 * this method adjust the size of the matrix ot be compatible with fourier
 * params : cv matrix object
 * returns : cv matrix object
 * it uses the zero padding to make the optimal size
 * as the fourier transform is done faster
 * when the size of the image is at the
 * powe of 2, 3 or 5.
*/
cv::Mat adjustSize(const cv::Mat& I)
{
    //expand input image to optimal size
    cv::Mat padded;
    int m = cv::getOptimalDFTSize( I.rows );
    int n = cv::getOptimalDFTSize( I.cols );
    // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    return padded;
}

/*
 * this function construct the complex matrix
 * params : array of matrix objects.
 * return : cv Mat object type.
 * merging the two matrices in one complex
 * matrix with the same shape of the
 * fourier result.
*/

cv::Mat constructComplexNumbers(const cv::Mat planes[]){
    cv::Mat complexI;
    merge(planes, 2, complexI);
    return complexI;
}
/*
 * this function prepare magnitude matrix to be displayed
 * params : array of matrices (the real and imaginary matrices)
 * rutern : the magnitude matrix.
 * adding one to all magnitude values as
 * if one value was zero it will cause
 * undefind behavior as it will result in
 * infinity when go under the logarithm
 * so adding one to all values we avoid this
 * type of error.
 * then it crops the spectrum, if it has an odd number of rows or columns
 * very triky when using the bitwise & :)
*/
cv::Mat prepareMagnitude(const cv::Mat planes[]){
    cv::Mat magI = planes[0];
    magI += cv::Scalar::all(1);
    // switch to logarithmic scale
    log(magI, magI);
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    return magI;
}

/*
 * this function split the magnitude matrix to 4 quarters.
 * params : magnitude CV matrix object.
 * return : array of 4 quarters.
 * it uses the Rect function to make 4 equal windows of
 * the magnitude matrix.
*/
std::array<cv::Mat,4> makeQuarters(const cv::Mat& magI)
{
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    std::array<cv::Mat,4> quarters = {q0,q1,q2,q3};
    return quarters;
}

/*
 * this function rearrange the 4 quadrants to
 * enhance the visualization of the image.
 * params : array of 4 quarters of mat type.
 * return : array of 4 quarters of mat type.
 * rearrange the quadrants of Fourier image
 * so that the origin is at the image center
*/

std::array<cv::Mat,4> reArrangeQuarters(const std::array<cv::Mat,4>& quarters)
{
    cv::Mat tmp;
    // swap quadrants (Top-Left with Bottom-Right)
    quarters[0].copyTo(tmp);
    quarters[3].copyTo(quarters[0]);
    tmp.copyTo(quarters[3]);
    // swap quadrant (Top-Right with Bottom-Left)
    quarters[1].copyTo(tmp);
    quarters[2].copyTo(quarters[1]);
    tmp.copyTo(quarters[2]);
    return quarters;
}

/*
 * this function prepare the magnitude matrix to
 * be displayed on the QT widgets.
 * params : cv matrix object.
 * return : cv matrix object.
 * it normalises the image to be [0,255]
 * and change the data type of the individual values.
*/
cv::Mat prepMatForConverting(const cv::Mat& src){
    cv::Mat temp = cv::Mat(src.size(),CV_8U);
    double  minVal,  maxVal;
    minMaxLoc(src,  &minVal,  &maxVal);
    src.convertTo(temp,CV_8U,255.0/(maxVal  -  minVal),  -minVal);
    return temp;
}
