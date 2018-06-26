#pragma once

#include <iostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.1415926

class LaneDetector
{    
public:
    LaneDetector();
    void applyBlur(cv::Mat &input, cv::Mat &output);
    void UndistXML(cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
    void Undist(cv::Mat frame_in, cv::Mat &frame_out, cv::Mat cameraMatrix, cv::Mat distCoeffs);
    void BirdEye(cv::Mat frame_in, cv::Mat &frame_out);
    void BirdEye_both(cv::Mat frame_in_white, cv::Mat &frame_out_white, cv::Mat &frame_in_yellow, cv::Mat &frame_out_yellow);
    void CreateTrackbars();
    static void on_yellow_H_trackbar(int, void*);
    static void on_white_H_trackbar(int, void*);
    static void on_yellow_S_trackbar(int, void*);
    static void on_white_S_trackbar(int, void*);
    static void on_yellow_V_trackbar(int, void*);
    static void on_white_V_trackbar(int, void*);
    static void on_yellow_thresh_trackbar(int, void*);
    static void on_white_thresh_trackbar(int, void*);
    static void on_accuracy_trackbar(int, void*);
    static void on_f_trackbar(int, void*);
    static void on_acc_trackbar(int,void*);
    static void on_acc_filt_trackbar(int,void*);
    //
    static void on_Rdown_trackbar(int,void*);
    static void on_Rup_trackbar(int,void*);
    static void on_Gdown_trackbar(int,void*);
    static void on_Gup_trackbar(int,void*);
    static void on_Bdown_trackbar(int,void*);
    static void on_Bup_trackbar(int,void*);
    //
    static void on_Hdown_trackbar(int,void*);
    static void on_Hup_trackbar(int,void*);
    static void on_Sdown_trackbar(int,void*);
    static void on_Sup_trackbar(int,void*);
    static void on_Vdown_trackbar(int,void*);
    static void on_Vup_trackbar(int,void*);
    //
    static void on_Hdown_trackbar_white(int,void*);
    static void on_Hup_trackbar_white(int,void*);
    static void on_Sdown_trackbar_white(int,void*);
    static void on_Sup_trackbar_white(int,void*);
    static void on_Vdown_trackbar_white(int,void*);
    static void on_Vup_trackbar_white(int,void*);


    void Hsv(cv::Mat frame_in, cv::Mat &yellow_frame_out, cv::Mat &white_frame_out, cv::Mat &yellow_canny, cv::Mat &white_canny);
    void Hsv_both(cv::Mat &frame_in, cv::Mat &yellow_frame_out, cv::Mat &white_frame_out, cv::Mat &yellow_canny, cv::Mat &white_canny);
    void colorTransform(cv::Mat &input, cv::Mat &output);
    void detectLine(cv::Mat &input, std::vector<std::vector<cv::Point>> &output);
    void detectLine_both(cv::Mat &input_white, std::vector<std::vector<cv::Point>> &output_white,cv::Mat &input_yellow, std::vector<std::vector<cv::Point>> &output_yellow);
    void drawPoints(std::vector<std::vector<cv::Point> > &input, cv::Mat &output);
    void drawPoints_both(std::vector<std::vector<cv::Point>> &input_white, cv::Mat &output_white,std::vector<std::vector<cv::Point>> &input_yellow, cv::Mat &output_yellow);
    void ConeDetection(cv::Mat frame_in, cv::Mat &frame_out, std::vector<cv::Point> &conePoints);
    void ConeDetection_new(cv::Mat frame_in, cv::Mat &frame_out, std::vector<cv::Point> &conePoints);
    std::vector<cv::Point> cones_vector;

private:
    //birdeye variables
    int alpha_;
    double focalLength;
    int dist_ = 500;
    int f_;

    double f, dist;
    double alpha;

    cv::Size taille;
    cv::Mat A1 ;
    cv::Mat RX ;
    cv::Mat R;
    cv::Mat T;
    cv::Mat A2;
    cv::Mat K ;

    //hsv variables
    cv::Mat imageHSV, yel_CannyHSV, whi_CannyHSV;
    cv::Mat kernel_v;
    cv::Point anchor = cv::Point(-1, -1);
    cv::Mat yel_Line_frame;
    cv::Mat whi_Line_frame;

    cv::Mat3b yel_hsv;
    cv::Mat3b whi_hsv;

    int yel_thresh;
    int whi_thresh;


    //detectline variables
    std::vector<cv::Vec4i> hierarchy_detectline;

    //cone detector variables
    cv::Mat image_HSV, img_thresh_low,img_thresh_high, result, frame_tmp;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat drawing;
    cv::Mat poly;
    cv::Mat dst;

    std::string label_cone = "cone";


};
