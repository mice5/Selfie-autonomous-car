#include "main.h"

using namespace std;
using namespace cv;

//camera params
const int Height = 480;
const int Width = 752;

//rectangle algorithm params
int number_of_rec_cols = 40;
int number_of_rec_raws = 10; //liczba pasów detekcji //slidery ustawiaja

//main MAT
Mat w_mat = Mat::zeros( Height, Width, CV_8UC3 ); //blank matri
Mat y_mat = Mat::zeros( Height, Width, CV_8UC3 );

Mat wy_test_mat = Mat::zeros(Height, Width, CV_8UC3 );
Mat wy_mat = Mat::zeros(Height, Width, CV_8UC3 );

Mat lidar_mat = Mat::zeros(600, 1000, CV_8UC3 );



//send average angle
uint32_t average_angle_counter = 0;
uint32_t angle_sum = 0;

int main(int argc, char** argv)
{

    #if defined(TEST_MODE) || defined (DEBUG_MODE)
    //init_trackbars();
    namedWindow("rect_trackbars");
    init_rect_trackbars("rect_trackbars");//trackbars for rect opt
    #endif


    //splines
    spline_t y_spline;
    spline_t trajectory_path;
    spline_t w_spline;

    //set default spline = go straigth
    vector<Point> straight_vector;
    straight_vector.push_back(Point(Width/2,Height));
    straight_vector.push_back(Point(Width/2,Height/2));
    straight_vector.push_back(Point(Width/2,0));

    y_spline.set_spline(straight_vector);
    w_spline.set_spline(straight_vector);
    trajectory_path.set_spline(straight_vector);


    //tangents
    tangent y_tangent;
    tangent w_tangent;
    tangent trajectory_tangent;


    //sharedmemory
    SharedMemory shm_lane_points(50002);
    vector<Point> w_point_vector; //vector for white line
    vector<Point> y_point_vector; //vector for yellow line
    vector<Point> c_point_vector; //vector for cons
    shm_lane_points.init(); //init

    SharedMemory shm_lidar_points(50001);
    vector<Point> l_point_vector; //vector for lidar points
    shm_lidar_points.init();


    SharedMemory shm_usb_to_stm(50003);
    vector<uint32_t> usb_from_vision(5);
    shm_usb_to_stm.init();



    //usb communication
    USB_STM USB_COM;
    data_container to_send;
    uint32_t velocity;
    uint32_t angle;
    USB_COM.init();//init

    init_trackbars();

    bool y_line_detect = 0;
    bool w_line_detect = 0;
////////////////////////////////////////////WHILE///////////////////////////////////////////////////
while(1)
{
    #if defined(TEST_MODE) || defined(DEBUG_MODE)
        number_of_rec_cols = rect_slider[0];
        number_of_rec_raws = rect_slider[1];
    #endif

    #ifdef DEBUG_MODE
    //clear buffer which could have another size
    w_point_vector.clear();
    y_point_vector.clear();
    c_point_vector.clear();
    l_point_vector.clear();

    //get new set of points
    shm_lane_points.pull_line_data(y_point_vector,w_point_vector,c_point_vector);
    shm_usb_to_stm.pull_usb_data(usb_from_vision);
    shm_lidar_points.pull_lidar_data(l_point_vector);

    //preview of received points
    points_preview(w_point_vector,wy_test_mat,CV_RGB(255,255,255));
    points_preview(y_point_vector,wy_test_mat,CV_RGB(255,255,0));
    points_preview(c_point_vector,wy_test_mat,CV_RGB(255,0,0));

    //preview of lidar
    points_preview(l_point_vector,lidar_mat,CV_RGB(255,0,255));
    rectangle(lidar_mat,Point(490,490),Point(510,510),CV_RGB(255,0,0));

    imshow("Podglad", wy_test_mat);
    imshow("Lidar",lidar_mat);

    y_line_detect = 0;
    w_line_detect = 0;

    //begin y condition
    if(y_point_vector.size()>10)
    {
        y_line_detect = 1;

        points_to_mat(y_mat,y_point_vector);
        rectangle_optimize(y_mat,y_spline);
    }
    if(w_point_vector.size()>10)
    {
        w_line_detect = 1;

        points_to_mat(w_mat,w_point_vector);
        rectangle_optimize(w_mat,w_spline);
    }


    //set path according to lines
    if(y_line_detect == 1 && w_line_detect == 1)
    {
       two_line_planner(y_spline,w_spline,0,trajectory_path);
       trajectory_tangent.calculate(trajectory_path,rect_slider[3]);
       trajectory_tangent.angle();
    }
    else if(y_line_detect)
    {
        one_line_planner(y_spline,0,trajectory_path);
        trajectory_tangent.calculate(trajectory_path,rect_slider[3]);
        trajectory_tangent.angle();
    }
    else if(w_line_detect)
    {
        one_line_planner(w_spline,0,trajectory_path);
        trajectory_tangent.calculate(trajectory_path,rect_slider[3]);
        trajectory_tangent.angle();
    }
    else
    {

    }



    #endif

    #ifdef TEST_MODE
        points_to_mat(test_mat,"/home/mateusz/GIT/Selfie-autonomous-car/cpp-opencv-app/trajectory-planning/trajectory-planning/selfie_testy/prosta_syf.txt");
        rectangle_optimize(test_mat,test_spline);
        one_line_planner(test_spline,rect_slider[4],test_path);
        test_tangent.calculate(test_path,rect_slider[3]);
    #endif


///////////////////////////////////////////////////////////////////////////////////////////
     #if defined(RACE_MODE) || defined(DEBUG_MODE)

     angle = (trajectory_tangent.angle_deg+30)*10;
     angle_sum+=angle;
     average_angle_counter++;

     if(average_angle_counter == 2)
     {
        uint32_t angle_to_send;
        angle_to_send = angle_sum/1;
        velocity = left_slider[0];
        velocity = 1000;
        //send data to STM
        USB_COM.data_pack(velocity,angle_to_send,usb_from_vision,&to_send);
        USB_COM.send_buf(to_send);

        //USB_COM.read_buf(50);

        //read data from STM


        angle_sum = 0;
        average_angle_counter = 0;
     }




    #endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
     //display section
    string label;

    #ifdef TEST_MODE

        //test display
        test_spline.draw(test_mat,CV_RGB(20,20,250));//draw white right line
        test_path.draw(test_mat,CV_RGB(225,20,100));
        test_tangent.draw(test_mat,CV_RGB(255,255,0));

        label  = "traj_ang: "+ std::to_string(test_tangent.angle_deg);
        putText(test_mat, label, Point(450, 40), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,0), 1.0);

        //show white line mat
        imshow("tryb TEST",test_mat);
        test_mat = Mat::zeros( 480, 640, CV_8UC3 );//zero matrix
    #endif

    #ifdef DEBUG_MODE

        add(y_mat,w_mat,wy_mat); // to see rectangles

        //draw detected spline
        if(y_line_detect)
        y_spline.draw(wy_mat,CV_RGB(255,255,0));
        if(w_line_detect)
        w_spline.draw(wy_mat,CV_RGB(255,255,255));

        trajectory_path.draw(wy_mat,CV_RGB(0,255,0));
        trajectory_tangent.draw(wy_mat,CV_RGB(100,100,100));


        label  = "traj_ang: "+ std::to_string(trajectory_tangent.angle_deg);
        putText(wy_mat, label, Point(450, 40), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,0), 1.0);

        //show white line mat
        imshow("tryb DEBUG",wy_mat);

        //clean matrixes
        wy_mat = Mat::zeros(Height,Width,CV_8UC3);
        w_mat = Mat::zeros(Height,Width,CV_8UC3);
        y_mat = Mat::zeros(Height,Width,CV_8UC3);
        wy_test_mat = Mat::zeros( Height, Width, CV_8UC3 );//zero matrix
        lidar_mat = Mat::zeros(600,1000, CV_8UC3 );

        if(waitKey(30)>=0)
            break;
    #endif
}//end of while(1)

   // shm_lane_points.close();
    return 0;

}

