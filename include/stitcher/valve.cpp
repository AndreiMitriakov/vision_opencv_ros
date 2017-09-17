#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include "finddistance.h"
#include "valve.h"
using namespace cv;
using namespace std;

#define PI 3.14159265

void getOrientation(Mat& src,  vector<vector<Point> >& oneCnt, double& angle);

void getDistance(Mat& src, Mat& dist, vector<vector<Point> >& oneCnt, vector<double >& crds);

void valve(Mat& src, Mat& dist, vector<vector<double > >& coords)
{

  //initisialisation//
  Mat res; 
  Point centroid;
  double angle;

  //--------------------------//


  vector<vector<Point> > oneCnt;
//  vector<double > crds;//coordinates//X, Y, Z, L
    
   getOrientation(src, oneCnt, angle);//it works, but why?

  if(oneCnt.size() != 0){

   getDistance(src, dist, oneCnt, coords);

   coords[0].push_back(angle);
  }


      
}

void getDistance(Mat& src, Mat& dist, vector<vector<Point> >& oneCnt, vector<vector<double > >& crds){

  // Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect( oneCnt.size() );
  minRect[0] = minAreaRect( Mat(oneCnt[0]) );
  //Drawing rotated rects

  
  int width = minRect[0].size.width;
  
  finddistance(src, dist, minRect, true, crds); 
   
}

void getOrientation(Mat& src, vector<vector<Point> >& oneCnt , double& angle){
  bool orientation = true;
  int max;
  vector<vector<Point> > cnt;
  //getCnt
  Mat src_treshold, src_hsv;
//  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src, src, Size(3,3) );
  //tresholding
  vector< int> HSVmin;
  HSVmin.push_back(144);//144
  HSVmin.push_back(30);//97
  HSVmin.push_back(30);//31
  vector< int> HSVmax;
  HSVmax.push_back(197);//197
  HSVmax.push_back(255);//239
  HSVmax.push_back(255);//120
  cvtColor( src, src_hsv, CV_BGR2HSV );
  
  dilate( src, src, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) ); 
  medianBlur ( src, src, 7 );
 
  inRange(src_hsv,Scalar(HSVmin[0],HSVmin[1],HSVmin[2]),Scalar(HSVmax[0],HSVmax[1],HSVmax[2]), src_treshold);

  vector<Vec4i> hierarchy;

  findContours( src_treshold, cnt, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  //moments
  if(cnt.size() != 0){
  vector<Moments> mu(cnt.size() );
  vector< int> areas(cnt.size() );
  for( size_t i = 0; i < cnt.size(); i++ ){
    mu[i] = moments( cnt[i], false );
    areas.push_back(mu[i].m00);
  }
  //finding of the largest one
  if(mu.size() != 0)
    for(int j = 0; j < mu.size(); j++){
      if(j == 0)
        max = 0;
      else
        if(mu[j].m00 > mu[max].m00)
          max = j;        
    }
  //getCentroid
  double X = mu[max].m10 / mu[max].m00 ;
  double Y = mu[max].m01 / mu[max].m00 ;
  
  drawContours( src, cnt, max, Scalar(0, 255, 0), 1, 8, hierarchy, 0, Point() ); 

  //boundingBox
  

  //getCentralMomentsNormalized
  double u20 = mu[max].m20 / mu[max].m00 - X*X;
  double u02 = mu[max].m02 / mu[max].m00 - Y*Y;
  double u11 = mu[max].m11 / mu[max].m00 - X*Y;
  double  eigVal1, eigVal2;
  double eigVec1[2], eigVec2[2];

  
  //eigVals
  double D = (u20+u02)*(u20+u02) -4*(u20*u02 - u11*u11);
  if(D > 0 ){
    eigVal1 = 0.5*(u02+u20 + sqrt(D) );
    eigVal2 = 0.5*(u02+u20 - sqrt((u20+u02)*(u20+u02) -4*(u20*u02 - u11*u11)));
  }
  else
    orientation = false;
  //eigVectors
  if(( (u20 - eigVal1) != 0) && ( (u20 - eigVal2) != 0) ){
    eigVec1[0] = - u11 / (u20 - eigVal1);
    eigVec1[1] = 1;
    double norm1 = sqrt(eigVec1[0]*eigVec1[0]+eigVec1[1]*eigVec1[1]);
    eigVec1[0] = eigVec1[0] / norm1;
    eigVec1[1] = eigVec1[1] / norm1;


   // angle = - atan(eigVec1[1] / eigVec1[0] ) * 90 ;
   // cout << "ANGLE "<< angle<<endl;
    eigVec2[0] = - u11 / (u20 - eigVal2);
    eigVec2[1] = 1;
    double norm2 = sqrt(eigVec2[0]*eigVec2[0]+eigVec2[1]*eigVec2[1]);
    eigVec2[0] = eigVec2[0] / norm2;
    eigVec2[1] = eigVec2[1] / norm2;

  }
  else
    orientation = false;
  
  if(orientation){
    Point pt2; pt2.x = X + eigVec1[0]*15; pt2.y = Y + eigVec1[1]*15;
    Point pt1; pt1.x = X - eigVec1[0]*15; pt1.y = Y - eigVec1[1]*15;
    line(src, pt1, pt2, Scalar( 0, 0, 255), 2, 8, 0);

    Point pT2; pT2.x = X + eigVec2[0]*5; pT2.y = Y + eigVec2[1]*5;
    Point pT1; pT1.x = X - eigVec2[0]*5; pT1.y = Y - eigVec2[1]*5;
    line(src, pT1, pT2, Scalar(0, 0, 255), 2, 8, 0);
  }
  else
    cout<<"Try it later"<<endl;
  oneCnt.push_back(cnt[max]);
  vector<RotatedRect> minRect( oneCnt.size() );
  minRect[0] = minAreaRect( Mat(oneCnt[0]) );

  if(eigVec1[0]>0)
    angle = -(minRect[0].angle + 90);
  else 
    angle = - minRect[0].angle;
//  cout<<"angle "<< angle<<endl;
  }
}

