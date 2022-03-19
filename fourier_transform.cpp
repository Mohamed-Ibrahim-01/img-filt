#include "fourier_transform.h"
Mat convertToFourier(Mat img)
{
    Mat padded = adjustSize(img);
    // Add to the expanded another plane with zeros
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI = constructComplexNumbers(planes);
    dft(complexI, complexI);
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magnitude = prepareMagnitude(planes);
    array<Mat,4> quarters = makeQuarters(magnitude);
    quarters = reArrangeQuarters(quarters);
    normalize(magnitude, magnitude, 0, 1, NORM_MINMAX);
    return magnitude;
}
Mat adjustSize(Mat I)
{
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values


    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    return padded;
}

Mat constructComplexNumbers(Mat planes[])
{
    Mat complexI;
    merge(planes, 2, complexI);
    return complexI;
}
Mat prepareMagnitude(Mat planes[])
{
    Mat magI = planes[0];
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
    return magI;
}

array<Mat,4> makeQuarters(Mat magI)
{
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
    array<Mat,4> quarters = {q0,q1,q2,q3};
    return quarters;
}
// rearrange the quadrants of Fourier image  so that the origin is at the image center
array<Mat,4> reArrangeQuarters(array<Mat,4> quarters)
{
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    quarters[0].copyTo(tmp);
    quarters[3].copyTo(quarters[0]);
    tmp.copyTo(quarters[3]);
    quarters[1].copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    quarters[2].copyTo(quarters[1]);
    tmp.copyTo(quarters[2]);
    return quarters;
}
// Transform the matrix with float values into a
// viewable image form (float between values 0 and 1).
// Show the result



