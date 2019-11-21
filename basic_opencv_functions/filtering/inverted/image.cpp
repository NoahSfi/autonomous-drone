#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
        //display the original image
        Mat img = imread("../../voiture.jpg");
        namedWindow("MyWindow");
        imshow("MyWindow", img);

        //invert and display the inverted image
        bitwise_not (img, img );
        namedWindow("Inverted");
        imshow("Inverted", img);

        waitKey(0);
      
        //cleaning up
        destroyAllWindows();
      
        return 0;
}

