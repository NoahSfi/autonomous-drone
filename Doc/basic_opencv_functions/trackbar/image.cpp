#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
     // Read original image 
     Mat src = imread("../../voiture.jpg");

     //if fail to read the image
     if (!src.data) 
     { 
          cout << "Error loading the image" << endl;
          return -1; 
     }

     // Create a window
     namedWindow("My Window", 1);

     //Create trackbar to change brightness
     int iSliderValue1 = 50;
     createTrackbar("Brightness", "My Window", &iSliderValue1, 100);

     //Create trackbar to change contrast
     int iSliderValue2 = 50;
     createTrackbar("Contrast", "My Window", &iSliderValue2, 100);

     while (true)
     {
          //Change the brightness and contrast of the image (For more infomation http://opencv-srf.blogspot.com/2013/07/change-contrast-of-image-or-video.html)
          Mat dst;
          int iBrightness  = iSliderValue1 - 50;
          double dContrast = iSliderValue2 / 50.0;
          src.convertTo(dst, -1, dContrast, iBrightness); 
          //show the brightness and contrast adjusted image

          imshow("My Window", dst);

          // Wait until user press some key for 50ms
          int iKey = waitKey(5);

          //if user press 'ESC' key
          if (iKey == 27)
          {
               break;
          }
     }

     return 0;
}