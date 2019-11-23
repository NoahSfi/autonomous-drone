// I'm considering rewriting video.cpp because the one from the ardrone cvdrone repository is too messy and broken.
// Here is what I've done so far...
// The following only works for AR Drone 2.0!

#include "ardrone.h"

int ARDrone::initVideo(void) {

        assert (version.major == ARDRONE_VERSION_2);
        
        // Open the IP address and port
        char filename[256];
        sprintf(filename, "tcp://%s:%d", ip, ARDRONE_VIDEO_PORT);
        if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) < 0) {
            CVDRONE_ERROR("avformat_open_input() failed. (%s, %d)\n", __FILE__, __LINE__);
            return 0;
        }
        return 1;
}

