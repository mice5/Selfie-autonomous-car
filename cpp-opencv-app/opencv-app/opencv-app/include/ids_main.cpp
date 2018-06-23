#include <iostream>
#include <stdio.h>
#include <stddef.h>

#include "opencv2/opencv.hpp"
#include "ueye.h"

#include "ids.h"

IDS_PARAMETERS ids_camera1;
HIDS hCam1 = 1;

int main_not()
{
    ids_camera1.initialize_camera(&hCam1);
    ids_camera1.setting_auto_params(&hCam1);
    ids_camera1.change_params(&hCam1);
    cvNamedWindow("frame", 1);
    cvNamedWindow("ids", 1);
    cvNamedWindow("ids2", 1);
    cv::createTrackbar("Pixel", "ids", &ids_camera1.pixelclock_slider, 80, NULL);
    cv::createTrackbar("Exposure", "ids", &ids_camera1.exposure_slider, 500, NULL);
    cv::createTrackbar("FPS", "ids", &ids_camera1.fps_slider, 90, NULL);
    cv::createTrackbar("Master", "ids2", &ids_camera1.Master_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Master", "ids2", 100);
    cv::createTrackbar("Green", "ids2", &ids_camera1.Green_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Green", "ids2",100);
    cv::createTrackbar("Red", "ids2", &ids_camera1.Red_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Red", "ids2", 100);
    cv::createTrackbar("Blue", "ids2", &ids_camera1.Blue_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Blue", "ids2", 100);
    cv::createTrackbar("Sharpness", "ids", &ids_camera1.sharpness_slider, 9, NULL);
    cv::setTrackbarMin("Sharpness", "ids", 0);
    cv::createTrackbar("Gamma", "ids", &ids_camera1.Gamma, 300, NULL);


    while (true) {

        cv::Mat ids_image (480, 752, CV_8UC3);
        ids_camera1.get_frame(&hCam1,752,480,ids_image);
        ids_camera1.update_params(&hCam1);
        std::cout << "FPS: "<<ids_camera1.NEWFPS<< std::endl;
        cv::imshow("frame", ids_image);

        if ((int)cv::waitKey(10) >= 0) {
            break;
        }
    }

    // Release the camera
    is_ExitCamera(hCam1);

    return 0;
}
