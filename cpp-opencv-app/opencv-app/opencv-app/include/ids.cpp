#include "ids.h"

//Function that initialize uEye camera
void IDS_PARAMETERS::initialize_camera(HIDS* hCam) {
	INT nRet = is_InitCamera(hCam, NULL);
	if (nRet == IS_SUCCESS) {
		std::cout << "Camera initialized!" << std::endl;
	}

	// Setting the pixels clock
	UINT nPixelClockDefault = 21;
	nRet = is_PixelClock(*hCam, IS_PIXELCLOCK_CMD_SET, (void*)&PixelClock, sizeof(PixelClock));

	if (nRet == IS_SUCCESS) {
		std::cout << "Camera pixel clock succes" << std::endl;
	}
	else if (nRet == IS_NOT_SUPPORTED) {
		std::cout << "Camera pixel clock setting is not supported" << std::endl;
	}

	// Set the color mode
    INT colorMode = IS_CM_BGR8_PACKED;
	nRet = is_SetColorMode(*hCam, colorMode);

	if (nRet == IS_SUCCESS) {
		std::cout << "Camera color mode succes" << std::endl;
	}

	INT displayMode = IS_SET_DM_DIB;
	nRet = is_SetDisplayMode(*hCam, displayMode);


    int retInt = is_AllocImageMem(*hCam, 752, 480, 24, &pMem, &memID);
    if (retInt != IS_SUCCESS){
        std::cout << "Error in allocating memory" << std::endl;
    }
    is_SetImageMem(*hCam, pMem, memID);
}


// Capture a frame from IDS
void IDS_PARAMETERS::get_frame(HIDS* hCam, int width, int height,cv::Mat& mat) {

    //is_SetImageMem(*hCam, pMem, memID);
    is_FreezeVideo(*hCam, IS_WAIT);

    VOID* pMem_b;
    int retInt = is_GetImageMem(*hCam, &pMem_b);
    if (retInt != IS_SUCCESS) {
        std::cout << "Image data could not be read from memory!" << std::endl;
    }
    else
        ;
        memcpy(mat.ptr(), pMem_b, width*height*3);
    //mat.ptr() = pMem_b;
    //is_FreeImageMem(*hCam, pMem, memID);
}

//Updating parameters from trackbars in while loop
void IDS_PARAMETERS::update_params(HIDS* hCam) {
	PixelClock = (UINT)pixelclock_slider;
	is_PixelClock(*hCam, IS_PIXELCLOCK_CMD_SET, (void*)&PixelClock, sizeof(PixelClock));

    Exposure = (double)(exposure_slider/10);
	is_Exposure(*hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&Exposure, sizeof(Exposure));

    FPS = (double)fps_slider;
	is_SetFrameRate(*hCam, FPS, &NEWFPS);

    is_SetHWGainFactor(*hCam, IS_SET_MASTER_GAIN_FACTOR, Master_GAIN_Factor);
    is_SetHWGainFactor(*hCam, IS_SET_GREEN_GAIN_FACTOR, Green_GAIN_Factor);
    is_SetHWGainFactor(*hCam, IS_SET_BLUE_GAIN_FACTOR, Blue_GAIN_Factor);
    is_SetHWGainFactor(*hCam, IS_SET_RED_GAIN_FACTOR, Red_GAIN_Factor);
	Sharpness = (INT)sharpness_slider;
	is_EdgeEnhancement(*hCam, IS_EDGE_ENHANCEMENT_CMD_SET, &Sharpness, sizeof(Sharpness));
	is_Gamma(*hCam, IS_GAMMA_CMD_SET, &Gamma, sizeof(Gamma));
}

//Enabling auto parameters
void IDS_PARAMETERS::setting_auto_params(HIDS* hCam) {
	double enable = 1;
	double disable = 0;
	is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_GAIN, &enable, 0);
    is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0);
    is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &enable, 0);
	is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_SHUTTER, &enable, 0);
    is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &enable, 0);
    is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &enable, 0);
	is_SetAutoParameter(*hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &enable, 0);
}

//Changing camera setting and gettign default variables
void IDS_PARAMETERS::change_params(HIDS *hCam) {
	is_Exposure(*hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&Exposure, sizeof(Exposure));

	is_SetFrameRate(*hCam, FPS, &NEWFPS);

	// Enable setting gain boost
	int nRet = is_SetGainBoost(*hCam, IS_SET_GAINBOOST_ON);
	if (nRet == IS_SUCCESS) {
		std::cout << "Enabling Gain success" << std::endl;
	}
	//Get gain factors
    Master_GAIN_Factor = is_SetHWGainFactor(*hCam, IS_GET_DEFAULT_MASTER_GAIN_FACTOR,100);
    Red_GAIN_Factor = is_SetHWGainFactor(*hCam, IS_GET_DEFAULT_RED_GAIN_FACTOR, 100);
    Green_GAIN_Factor = is_SetHWGainFactor(*hCam, IS_GET_DEFAULT_GREEN_GAIN_FACTOR, 100);
    Blue_GAIN_Factor = is_SetHWGainFactor(*hCam, IS_GET_DEFAULT_BLUE_GAIN_FACTOR, 100);

	nRet = is_EdgeEnhancement(*hCam, IS_EDGE_ENHANCEMENT_CMD_GET_DEFAULT, &Sharpness, sizeof(Sharpness));
	if (nRet == IS_SUCCESS) {
		std::cout << "Edge enhancement default success" << std::endl;
	}
	sharpness_slider = (UINT)Sharpness;

    //nRet = is_Gamma(*hCam, IS_GAMMA_CMD_GET_DEFAULT, &Gamma, sizeof(Gamma));
    //if (nRet == IS_SUCCESS) {
    //	std::cout << "Gamma default success" << std::endl;
    //}

}

//Creating in debug mode trackbars
void IDS_PARAMETERS::create_trackbars(void){
    cvNamedWindow("ids", 1);
    cv::createTrackbar("Pixel", "ids", &pixelclock_slider, 40, NULL);
    cv::createTrackbar("Exposure", "ids", &exposure_slider, 500, NULL);
    cv::createTrackbar("FPS", "ids", &fps_slider, 87, NULL);
    cv::createTrackbar("Master", "ids", &Master_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Master", "ids", 100);
    cv::createTrackbar("Green", "ids", &Green_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Green", "ids",100);
    cv::createTrackbar("Red", "ids", &Red_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Red", "ids", 100);
    cv::createTrackbar("Blue", "ids", &Blue_GAIN_Factor, 300, NULL);
    cv::setTrackbarMin("Blue", "ids", 100);
    cv::createTrackbar("Sharpness", "ids", &sharpness_slider, 9, NULL);
    cv::setTrackbarMin("Sharpness", "ids", 0);
    cv::createTrackbar("Gamma", "ids", &Gamma, 300, NULL);
}
