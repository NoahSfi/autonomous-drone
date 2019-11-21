#include "ardrone/ardrone.h"

// --------------------------------------------------------------------------
// main
// Description  : This is the entry point of the program.
// Return value : SUCCESS: 0  ERROR: -1
// --------------------------------------------------------------------------

int main() {

    // AR Drone class
    ARDrone ardrone;

    // Initialization
    if (!ardrone.open()) {
        printf("Failed to initialize.\n");
        return -1;
    }

    // Battery
    printf("Battery = %d%%\n", ardrone.getBatteryPercentage());

    // Update
    // if (!ardrone.update()) break;

    // Get an image
    // cv:Mat *image = ardrone.getImage();

    // Movement
    // ardrone.takeoff()
    // ardrone.landing();
    // ardrone.move3D(vx, vy, vz, vr)
    // ardrone.onGround()

    // Change Camera
    // static int mode = 0;
    // ardrone.setCamera(++mode%4)

    // Display Image
    // cv::imshow("camera", image);
    
    ardrone.close();
    return 0;
}