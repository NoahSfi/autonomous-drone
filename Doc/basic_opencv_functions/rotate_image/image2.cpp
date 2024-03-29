#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int iAngle = 180;
int iScale = 50;
int iBorderMode = 0;
Mat imgOriginal ;
int iImageCenterY = 0;
int iImageCenterX = 0;
const char* pzRotatedImage = "Rotated Image";

void CallbackForTrackBar(int, void*)
{
    Mat matRotation = getRotationMatrix2D(  Point( iImageCenterX, iImageCenterY ), (iAngle - 180), iScale / 50.0 );
    
    // Rotate the image
    Mat imgRotated;
    warpAffine( imgOriginal, imgRotated, matRotation, imgOriginal.size(), INTER_LINEAR, iBorderMode, Scalar() );

    imshow( pzRotatedImage, imgRotated );
 
}

 int main( int argc, char** argv )
 {
    // Load the image
    imgOriginal = imread( "../voiture.jpg", 1 );

    iImageCenterY = imgOriginal.rows / 2;
    iImageCenterX = imgOriginal.cols / 2;

    //show the original image
    const char* pzOriginalImage = "Original Image";
    namedWindow( pzOriginalImage, WINDOW_AUTOSIZE );
    imshow( pzOriginalImage, imgOriginal );

    //create the "Rotated Image" window and 3 trackbars in it
    namedWindow( pzRotatedImage, WINDOW_AUTOSIZE );
    createTrackbar("Angle", pzRotatedImage, &iAngle, 360, CallbackForTrackBar);
    createTrackbar("Scale", pzRotatedImage, &iScale, 100, CallbackForTrackBar);
    createTrackbar("Border Mode", pzRotatedImage, &iBorderMode, 5, CallbackForTrackBar);
    
    int iDummy = 0;

    CallbackForTrackBar(iDummy, &iDummy);

    waitKey(0);

 return 0;
}