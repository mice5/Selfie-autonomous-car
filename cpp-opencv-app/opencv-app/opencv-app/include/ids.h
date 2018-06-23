#ifndef IDS_H
#define IDS_H

#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include "opencv2/opencv.hpp"
#include "ueye.h"

#define IDS_WIDTH 480
#define IDS_HEIGHT 752

class IDS_PARAMETERS {
public:

    UINT PixelClock = 40;
    int pixelclock_slider = 40;
    double Exposure = 10.0;
    int exposure_slider = 100;
    double FPS = 87, NEWFPS;
    int fps_slider = 87;
    int Master_GAIN_Factor=300;
    int Red_GAIN_Factor=112;
    int Green_GAIN_Factor=100;
    int Blue_GAIN_Factor=158;
	
	int nRange[3];
    int sharpness_slider=1;
    UINT Sharpness=1;

    int Gamma =170;

    char* pMem = NULL;
    int memID = 0;

    void get_frame(HIDS* hCam, int width, int height,cv::Mat& mat);
	void initialize_camera(HIDS* hCam);
	void change_params(HIDS* hCam);
	void setting_auto_params(HIDS* hCam);
	void update_params(HIDS *hCam);
    void create_trackbars(void);

};


#endif
