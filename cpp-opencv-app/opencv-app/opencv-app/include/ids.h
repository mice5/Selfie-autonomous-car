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
	UINT PixelClock = 21;
	int pixelclock_slider = 21;
    double Exposure = 33.0;
    int exposure_slider = 330;
    double FPS = 30, NEWFPS;
    int fps_slider = 30;
	int Master_GAIN_Factor;
	int Red_GAIN_Factor;
	int Green_GAIN_Factor;
	int Blue_GAIN_Factor;
	
	int nRange[3];
	int sharpness_slider;
	UINT Sharpness;

    int Gamma =200;

	void get_frame(HIDS* hCam, int width, int height,cv::Mat& mat);
	void initialize_camera(HIDS* hCam);
	void change_params(HIDS* hCam);
	void setting_auto_params(HIDS* hCam);
	void update_params(HIDS *hCam);
    void create_trackbars(void);

};


#endif
