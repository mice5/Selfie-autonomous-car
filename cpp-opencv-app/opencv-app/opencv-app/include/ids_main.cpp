#include <iostream>

#include <include/ids.h>

IDS_PARAMETERS ids_c;
HIDS hCa = 1;

int main_not(void){
    struct timespec start, end;
    unsigned int licznik_czas = 0;
    float seconds = 0;
    float fps = 0;
    cv::Mat ids_image(480,752,CV_8UC3);
    cv::Mat frame_ids(480,752,CV_8UC3);
    ids_c.initialize_camera(&hCa, ids_image);
    ids_c.setting_auto_params(&hCa);
    ids_c.change_params(&hCa);

    ids_c.create_trackbars();
    cvNamedWindow("frame_ids",1);
    cvNamedWindow("fram",1);

    while(true)
    {

        if(licznik_czas == 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
        cv::namedWindow( "fram", WINDOW_AUTOSIZE );
        HWND* window = cv::cvGetWindowHandle("fram");
        ids_c.get_frame(&hCa,480,752,ids_image, window);
//        ids_c.update_params(&hCa);

//        cv::imshow("fram",ids_image);

        if(licznik_czas > 500)
        {
            licznik_czas = 0;
            clock_gettime(CLOCK_MONOTONIC, &end);
            seconds = (end.tv_sec - start.tv_sec);
            fps  =  1 / (seconds / 500);
//            std::cout <<"FPS: " << fps << std::endl;
            float fps;
			if (is_GetFramesPerSecond(hCa, &fps) == IS_SUCCESS)
				std::cout << "FPS: " << fps << std:: endl;
			std::cout << "BW: " << is_GetUsedBandwidth (hCa) << std:: endl;
        }
        else
        {
            licznik_czas++;
        }
        cv::waitKey(1);



    }

    is_FreeImageMem(hCa, ids_c.pMem, ids_c.memID);
    is_ExitCamera(hCa);


    return 0;
}

