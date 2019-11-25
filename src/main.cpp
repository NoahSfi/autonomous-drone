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
    printf("*    'W'     -- Move forwar           *\n");
    printf("*    'S'     -- Move backwar          *\n");
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
        if (key == 'q')      vz =  1.0;
        if (key == 'e')      vz = -1.0;
        ardrone.move3D(vx, vy, vz, vr);

        // Change camera
        static int mode = 0;
        if (key == 'c') ardrone.setCamera(++mode%4);

        // Display the image
        imshow("camera", image);
    }

    // See you
    ardrone.close();

    return 0;
}