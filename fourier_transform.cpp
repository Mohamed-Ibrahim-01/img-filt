#include "fourier_transform.h"
Mat convertToFourier(Mat img)
{
    // the padded object is in the optimal size
    // for fourier transform
    Mat padded = adjustSize(img);
    // Add to the expanded another plane with zeros
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    /*
     * merging the two matrices in one complex
     * matrix with the same shape of the
     * fourier result.
    */
    Mat complexI = constructComplexNumbers(planes);
    // applying the fourier transform on
    // the new complex matrix
    dft(complexI, complexI);
    /*
    * splitting the real and the imaginary
    * components of the complex fourier coeffecients.
    * planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    */
    split(complexI, planes);
    // calculating the magnitude as squareRoot(real^2 +imag^2)
    // planes[0] = magnitude
    magnitude(planes[0], planes[1], planes[0]);
    // preparing the magnitude matrix to be displayed
    Mat magnitude = prepareMagnitude(planes);
    // split the magnitude image into 4 quarters.
    array<Mat,4> quarters = makeQuarters(magnitude);
    /*
    * rearrangement of the four quarters
    * to enhance the visualisation as the
    *(0,0) frequency is now in the center.
    */
    quarters = reArrangeQuarters(quarters);
    /*
    * normalize the values of the displayed
    * image to be compatible with the displaying
    *functionality.
    */
    normalize(magnitude, magnitude, 0, 1, NORM_MINMAX);
    return magnitude;
}
Mat adjustSize(Mat I)
{
    //expand input image to optimal size
    Mat padded;
    /*
    * as the fourier transform is done faster
    * when the size of the image is at the
    * powe of 2, 3 or 5.
    */
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols );
    // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
    return padded;
}

Mat constructComplexNumbers(Mat planes[])
{
    // merging the 2 matrices in one complex matrix
    Mat complexI;
    merge(planes, 2, complexI);
    return complexI;
}
Mat prepareMagnitude(Mat planes[])
{
    Mat magI = planes[0];
    /*
    * adding one to all magnitude values as
    * if one value was zero it will cause
    * undefind behavior as it will result in
    * infinity when go under the logarithm
    * so adding one to all values we avoid this
    * type of error.
    */
    magI += Scalar::all(1);
    // switch to logarithmic scale
    log(magI, magI);
    // crop the spectrum, if it has an odd number of rows or columns
    // very triky when using the bitwise and :)
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

array<Mat,4> reArrangeQuarters(array<Mat,4> quarters)
{
    // rearrange the quadrants of Fourier image
    //so that the origin is at the image center
    Mat tmp;
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



