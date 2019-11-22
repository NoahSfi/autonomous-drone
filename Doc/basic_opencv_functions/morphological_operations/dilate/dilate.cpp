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

        //dilate and display the dilated image
        dilate(img, img, getStructuringElement(MORPH_RECT, Size(5, 5)));
        namedWindow("Dilated");
        imshow("Dilated", img);

        waitKey(0);
       
        //cleaning up
        destroyAllWindows();
       
        return 0;
}