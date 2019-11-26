#include "opencv2/opencv.hpp"
#include "camera.hpp"

cv::Mat undistortedImage(cv::Mat image) {
    cv::Mat cameraMatrix, distCoeffs;

    // read calibration settings from file
    cv::FileStorage fs("out_camera_data.xml", cv::FileStorage::READ);
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;

    // undistort image
    cv::Mat undistorted_image;
    undistort(image, undistorted_image, cameraMatrix, distCoeffs);
    return undistorted_image;
}

int main() {
    cv::VideoCapture camera(4);
    if (!camera.isOpened()) {
        return 1;
    }
    // create window
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    // capture image
    cv::Mat image;
    camera >> image;

    // display undistorted image
    cv:: Mat undistorted = undistortedImage(image);
    while (1) {
        cv::imshow("Webcam", undistorted);
        if (cv::waitKey(10) == 27) break;

    }
    return 0;
}