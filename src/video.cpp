// I'm considering rewriting video.cpp because the one from the ardrone cvdrone repository is too messy and broken.
// Here is what I've done so far...
// The following only works for AR Drone 2.0!

#include "ardrone/ardrone.h"

int ARDrone::initVideo(void) {

    assert (version.major == ARDRONE_VERSION_2);
    
    // Open the IP address and port
    char filename[256];
    sprintf(filename, "tcp://%s:%d", ip, ARDRONE_VIDEO_PORT);
    if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) < 0) {
        CVDRONE_ERROR("avformat_open_input() failed. (%s, %d)\n", __FILE__, __LINE__);
        return 0;
    }

    // Retrive and dump stream information
    avformat_find_stream_info(pFormatCtx, NULL);
    av_dump_format(pFormatCtx, 0, filename, 0);

    // Find the decoder for the video stream
    pCodecCtx = pFormatCtx->streams[0]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        CVDRONE_ERROR("avcodec_find_decoder() failed. (%s, %d)\n", __FILE__, __LINE__);
        return 0;
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        CVDRONE_ERROR("avcodec_open2() failed. (%s, %d)\n", __FILE__, __LINE__);
        return 0;
    }

    // Allocate video frames and a buffer
    pFrame = av_frame_alloc();
    pFrameBGR = av_frame_alloc();
    bufferBGR = (uint8_t*)av_mallocz(avpicture_get_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height) * sizeof(uint8_t));

    // Assign appropriate parts of buffer to image planes in pFrameBGR
    avpicture_fill((AVPicture*)pFrameBGR, bufferBGR, AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);

    // Convert it to BGR
    pConvertCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24, SWS_SPLINE, NULL, NULL, NULL);

    // Allocate an Image
    img = cv::Mat(cv::Size(pCodecCtx->width, (pCodecCtx->height == 368) ? 360 : pCodecCtx->height), CV_8UC3);

    // Clear the image
    img = cv::Scalar(5,10,15);

    // Create a mutex
    mutexVideo = new pthread_mutex_t;
    pthread_mutex_init(mutexVideo, NULL);

    // Create a thread
    threadVideo = new pthread_t;
    if (pthread_create(threadVideo, NULL, runVideo, this) != 0) {
        CVDRONE_ERROR("pthread_create() was failed. (%s, %d)\n", __FILE__, __LINE__);
        return 0;
    }

    return 1;
}

void ARDrone::loopVideo(void)
{
    while (1) {
        // Get video stream
        if (!getVideo()) break;
        pthread_testcancel();
        msleep(1);
    }
}

int ARDrone::getVideo(void){

    assert (version.major == ARDRONE_VERSION_2);

    AVPacket packet;
    int frameFinished = 0;

    // Read all frames
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Decode the frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        // Decoded all frames
        if (frameFinished) {
            // Convert to BGR
            if (mutexVideo) pthread_mutex_lock(mutexVideo);
            sws_scale(pConvertCtx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameBGR->data, pFrameBGR->linesize);
            if (mutexVideo) pthread_mutex_unlock(mutexVideo);

            // Free the packet and break
            av_free_packet(&packet);
            return 1;
            //break;
        }

        // Free the packet
        av_free_packet(&packet);
    }
    return 0;
}

cv::Mat ARDrone::getImage(void){

    // There is no image
    // if (!img) return NULL;

    // Enable mutex lock
    if (mutexVideo) pthread_mutex_lock(mutexVideo);

    // AR.Drone 2.0
    if (version.major == ARDRONE_VERSION_2) {
        memcpy(img.data, pFrameBGR->data[0], pCodecCtx->width * ((pCodecCtx->height == 368) ? 360 : pCodecCtx->height) * sizeof(uint8_t)*3);
    }

    // Disable mutex lock
    if (mutexVideo) pthread_mutex_unlock(mutexVideo);

    return img;
}


void ARDrone::finalizeVideo(void)
{
    // Destroy the thread
    if (threadVideo) {
        pthread_cancel(*threadVideo);
        pthread_join(*threadVideo, NULL);
        delete threadVideo;
        threadVideo = NULL;
    }

    // Delete the mutex
    if (mutexVideo) {
        pthread_mutex_destroy(mutexVideo);
        delete mutexVideo;
        mutexVideo = NULL;
    }

    // Release the Image
    img.release();

    // Deallocate the frame
    if (pFrame) {
        av_free(pFrame);
        pFrame = NULL;
    }

    // Deallocate the frame
    if (pFrameBGR) {
        av_free(pFrameBGR);
        pFrameBGR = NULL;
    }

    // Deallocate the buffer
    if (bufferBGR) {
        av_free(bufferBGR);
        bufferBGR = NULL;
    }

    // Deallocate the convert context
    if (pConvertCtx) {
        sws_freeContext(pConvertCtx);
        pConvertCtx = NULL;
    }

    // Deallocate the codec
    if (pCodecCtx) {
        avcodec_close(pCodecCtx);
        pCodecCtx = NULL;
    }

    // Deallocate the format context
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
        pFormatCtx = NULL;
    }
}