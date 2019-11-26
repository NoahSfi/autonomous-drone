#include "ardrone/ardrone.h"

using namespace cv;

int main(int argc, char **argv)
{
    // AR.Drone class
    ARDrone ardrone;

    // Initialize
    if (!ardrone.open()) {
        printf("Failed to initialize.\n");
        return -1;
    }

    // Battery
    printf("Battery = %d%%\n", ardrone.getBatteryPercentage());

    // Instructions
    printf("***************************************\n");
    printf("*       CV Drone sample program       *\n");
    printf("*           - How to Play -           *\n");
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("* - Controls -                        *\n");
    printf("*    'Space' -- Takeoff/Landing       *\n");
    printf("*    'W'     -- Move forward          *\n");
    printf("*    'S'     -- Move backward         *\n");
    printf("*    'A'     -- Turn left             *\n");
    printf("*    'D'     -- Turn right            *\n");
    printf("*    'Q'     -- Move upward           *\n");
    printf("*    'E'     -- Move downward         *\n");
    printf("*                                     *\n");
    printf("* - Others -                          *\n");
    printf("*    'C'     -- Change camera         *\n");
    printf("*    'Esc'   -- Exit                  *\n");
    printf("*                                     *\n");
    printf("***************************************\n\n");

    // TRACKING STUFF

    namedWindow("Control", WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 170;
    int iHighH = 179;

    int iLowS = 150; 
    int iHighS = 255;

    int iLowV = 60;
    int iHighV = 255;

    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);

    int iLastX = -1; 
    int iLastY = -1;

    //Capture a temporary image from the camera
    Mat imgTmp = ardrone.getImage(); 

    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );

    // END TRACKING STUFF

    while (1) {

        // Key input
        int key = waitKeyEx(33);
        if (key == 0x1b) break;

        // Update
        if (!ardrone.update()) break;

        // Get an image
        Mat image = ardrone.getImage();

        // Take off / Landing 
        if (key == ' ') {
            if (ardrone.onGround()) ardrone.takeoff();
            else                    ardrone.landing();
        }

        // Manual Move
        double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;
        if (key == 'w') vx =  1.0;
        if (key == 's') vx = -1.0;
        if (key == 'a') vr =  1.0;
        if (key == 'd') vr = -1.0;
        if (key == 'q') vz =  1.0;
        if (key == 'e') vz = -1.0;
        // ardrone.move3D(vx, vy, vz, vr);

        // Change camera
        static int mode = 0;
        if (key == 'c') ardrone.setCamera(++mode%4);


        Mat imgHSV;

        cvtColor(image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        
        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
            
        // morphological opening (removes small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        // morphological closing (removes small holes from the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        // Calculate the moments of the thresholded image
        Moments oMoments = moments(imgThresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;

        // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
        if (dArea > 10000){
            //calculate the position of the ball
            int posX = dM10 / dArea;
            int posY = dM01 / dArea;     
                
            if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0){
                //Draw a red line from the previous point to the current point
                line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
            }

            iLastX = posX;
            iLastY = posY;

            // Send Commands To The Drone
            double vtx = 0.0, vty = 0.0, vtz = 0.0, vtr = 0.0;
            int mx = 320;
            int my = 180;
            int mArea = 2000000;
            vtz = 2*(mx - posY)/mx;
            vty = 2*(my - posX)/my;
            vtr = 2*(mx - posX)/mx;
            vtx = 2*(mArea - dArea)/mArea; // Not sure about this one...
            ardrone.move3D(vtx+vx, vty+vy, vtz+vz, vtr+vr);
        }

        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        image = image + imgLines;

        imshow("Camera", image); //show the original image
        
        // END TRACKING STUFF

        // Display the image
        // imshow("Camera", image);
    }

    // See you
    ardrone.close();

    return 0;
}