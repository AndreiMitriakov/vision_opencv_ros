#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "stitcher/camera_poi.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float32.h"
#include "stitcher/valve.h"
#include "stitcher/videoOCR.h"
#include "geometry_msgs/Twist.h"

namespace enc = sensor_msgs::image_encodings;
  
static const char WINDOW[] = "Image Processed";
static const char WINDOW2[] = "Image dist";
static const char WINDOW3[] = "Image cnt";

image_transport::Publisher pub;
ros::Publisher pose_pub;
//ros::Publisher poi_pub;
ros::Publisher poi_pub;

stitcher::camera_poi msg_poi;

geometry_msgs::Twist msg_pose;
cv_bridge::CvImagePtr cv_ptr;
cv_bridge::CvImagePtr cv_ptr2;
bool valve_;


int return_number_of_color(string curClr) {
  if(curClr == "red")
    return 0;
  if(curClr == "orange")
    return 1;
  if(curClr == "yellow")
    return 2;
  if(curClr == "green")
    return 3;
  if(curClr == "aqua")
    return 4;
  if(curClr == "blue")
    return 5;
  if(curClr == "purple")
    return 6;
  if(curClr == "pink")
    return 7;
  if(curClr == "red")
    return 8;
  if(curClr == "black")
    return 9;
  if(curClr == "white")
    return 10;
  if(curClr == "circle")
    return 11;
  if(curClr == "triangle")
    return 12;
  if(curClr == "valve")
    return 13;
}
using namespace cv;

void imageCallback(const sensor_msgs::ImageConstPtr& original_image)
{
    vector<vector<double > > coords_valve;
    vector<vector<double > > coords_rect;
    vector<string > names;
    vector<int > namesInt;
    cv::Mat dist;
    cv::Mat cnt;
    cv::Mat imgValve;
    vector<int > resultOCR;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(original_image, enc::BGR8);

        cv_ptr2 = cv_bridge::toCvCopy(original_image, enc::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("tutorialROSOpenCV::main.cpp::cv_bridge exception: %s", e.what());
        return;
    }
//cout<<"------------------START------------------"<<endl;

//cout<<"Before videoOCR"<<endl;
    videoOCR(cv_ptr->image, dist, cnt, coords_rect, names, resultOCR);
    cv::imshow(WINDOW3, cnt);
    cv::imshow(WINDOW2, dist);

//cout<<"After videoOCR, before valve. Size of cv_ptr2->image "<< cv_ptr2->image.size() << " SIZE OF imgValve "<< imgValve.size() <<endl;

    if(valve_ == true){
      valve(cv_ptr2->image, imgValve, coords_valve);
//cout<<"After valve " <<  " SIZE OF imgValve "<< imgValve.size[0] <<endl;
//      names.push_back("valve");
    if((imgValve.size[0]  != 0)&& (imgValve.size[1]  != 0) ) 
        cv::imshow(WINDOW, imgValve);
    }

//cout<<"After valve"<<endl;
    for(int i = 0; i < names.size(); i++){
      for(int j = 0; j < names.size(); j++){
        namesInt.push_back( return_number_of_color(names[j]) );

 	if( (namesInt[j] == 12) && (valve_ == false) )
	  valve_ = true;
      }
    }
//cout<<"After renaming"<<endl;

    if((coords_valve.size()!=0)&&(valve_ == true)){
      msg_pose.linear.x = coords_valve[0][0];
      msg_pose.linear.y = coords_valve[0][1];
      msg_pose.linear.z = coords_valve[0][2];
      msg_pose.angular.x = coords_valve[0][3];
      pose_pub.publish(msg_pose);
    }
//cout<<"After msg pose"<<endl;
    if((coords_rect.size()!=0)){
      for(int k = 0; k < coords_rect.size(); k++){
        msg_poi.position[0] = coords_rect[k][0];
        msg_poi.position[1] = coords_rect[k][1];
        msg_poi.position[2] = coords_rect[k][2];
        msg_poi.name = namesInt[k];
	if(k < resultOCR.size())
          msg_poi.result = resultOCR[k];
	else
          msg_poi.result = 0;
        poi_pub.publish(msg_poi);
      }
    }


    pub.publish(cv_ptr->toImageMsg());
//cout<<"After msg poi"<<endl;
    //part of rect messages
//cout<<"------------------FINISH------------------"<<endl;
    //names: from string to enum

    cv::waitKey(3);

}
  
int main(int argc, char **argv)
{


    valve_ = false;
    ros::init(argc, argv, "image_processor");
    ros::NodeHandle nh;
    ros::Rate loop_rate (10) ;
    image_transport::ImageTransport it(nh);

    cv::namedWindow(WINDOW, CV_WINDOW_AUTOSIZE);
    cv::moveWindow(WINDOW, 1200, 50);
    cv::namedWindow(WINDOW2, CV_WINDOW_AUTOSIZE);
    cv::moveWindow(WINDOW2, 1200, 650);
    cv::namedWindow(WINDOW3, CV_WINDOW_AUTOSIZE);
    cv::moveWindow(WINDOW3, 600, 50);
    pose_pub = nh.advertise<geometry_msgs::Twist>("/cam_set_point", 1000);	  
    poi_pub = nh.advertise<stitcher::camera_poi>("/camera_poi", 1000);	
    image_transport::Subscriber sub = it.subscribe("usb_cam/image_raw", 1, imageCallback);

    cv::destroyWindow(WINDOW);
    cv::destroyWindow(WINDOW2);
    cv::destroyWindow(WINDOW3);

    pub = it.advertise("usb_cam/image_processed", 1);

    //ROS_INFO is the replacement for printf/cout.
    ROS_INFO("tutorialROSOpenCV::main.cpp::No error.");

  while (ros::ok ()) {
    ros::spinOnce () ;    /* Let pending callbacks execute. */
    loop_rate.sleep () ;  /* Sleep to run at amost at 10Hz. */
  }

}
