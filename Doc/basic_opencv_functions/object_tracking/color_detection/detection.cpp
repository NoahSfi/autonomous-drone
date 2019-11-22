#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main()
 {
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", WINDOW_AUTOSIZE); //create a window called "Control"

    //HUE represents the color, SATURATION  represents the amount to which that respective color is mixed with
    // white and VALUE  represents the  amount to which that respective color is mixed with black.
    int iLowH = 0;
    int iHighH = 0;

    int iLowS = 0; 
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    // Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

    Mat imgHSV;

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    
    Mat imgThresholded,imgThresholded2;

    inRange(imgHSV, Scalar(0, 120, 70), Scalar(10, 255, 255), imgThresholded); //Threshold the image
    inRange(imgHSV, Scalar(170, 120, 70), Scalar(180, 255, 255), imgThresholded2);

    imgThresholded += imgThresholded2;
    //morphological opening (remove small objects from the foreground)
    // erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    morphologyEx(imgThresholded, imgThresholded,MORPH_OPEN,getStructuringElement(MORPH_ELLIPSE, Size(5, 5)),Point(-1,1),2);

    //morphological closing (fill small holes in the foreground)
    // dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    // erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    morphologyEx(imgThresholded, imgThresholded,MORPH_CLOSE,getStructuringElement(MORPH_ELLIPSE, Size(5, 5)),Point(-1,1),2);

    imshow("Thresholded Image", imgThresholded); //show the thresholded image
    imshow("Original", imgOriginal); //show the original image

            if (waitKey(5) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
                cout << "esc key is pressed by user" << endl;
                break; 
        }
        }

    return 0;

}