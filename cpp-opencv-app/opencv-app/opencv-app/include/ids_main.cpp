#include <iostream>

#include <include/ids.h>
#include <pthread.h>

int main_not(void){
    struct timespec start, end;
    unsigned int licznik_czas = 0;
    float seconds = 0;
    float fps = 0;
    cv::Mat frame(IDS_HEIGHT, IDS_WIDTH, CV_8UC3);   
    cvNamedWindow("fram",1);

    ids.init();

    while(true)
    {

        pthread_cond_wait(&algorithm_signal, &algorithm_signal_mutex);
        static int denom = 0;
        if(++denom > 10){
            denom = 0;
            if(licznik_czas == 0)
            {
                clock_gettime(CLOCK_MONOTONIC, &start);
            }

            pthread_mutex_lock(&ids.frame_mutex);
            ids.ids_frame.copyTo(frame);
            pthread_mutex_unlock(&ids.frame_mutex);

            char s[20];
            sprintf(s, "FPS: %lf", ids.getFPS());
            cv::putText(frame, s, cv::Point(IDS_WIDTH/2, IDS_HEIGHT-30), CV_FONT_HERSHEY_DUPLEX, 0.4, CV_RGB(0, 255, 0), 1.0);
            cv::imshow("fram",frame);
            if(licznik_czas > 50)
            {
                licznik_czas = 0;
                clock_gettime(CLOCK_MONOTONIC, &end);
                seconds = (end.tv_sec - start.tv_sec);
//                fps  =  10 / (seconds / 50);
                fps = ids.getFPS();
                std::cout <<"center_pixel: " << frame.at<int>(IDS_WIDTH/2, IDS_HEIGHT/2) << std::endl;
                std::cout <<"FPS: " << fps << std::endl;
            }
            else
            {
                licznik_czas++;
            }

            cv::waitKey(10);
        }



    }
    ids.exit();


    return 0;
}

