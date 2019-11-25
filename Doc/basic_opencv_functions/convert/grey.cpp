#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv){

    //Read your image
    Mat image = imread("name of your image");

    // Check for failure
    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    Mat imageGrey;
    cvtColor(image, imageGrey, COLOR_BGR2GRAY);
    bool isSuccess = imwrite("imageGrey.jpg", imageGrey); 
    if (isSuccess == false)
    {
    cout << "Failed to save the image" << endl;
    cin.get(); //wait for a key press
    return -1;
    }

    cout << "Image is succusfully saved to a file" << endl;

     //destroy the created window

    return 0;

}