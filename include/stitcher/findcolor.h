#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <ros/ros.h>
#include <stdlib.h>
#include <string>  
#include <fstream>
//#include <aruco.hpp> 

//#include "transfUNI.h"
using namespace cv;
using namespace std;


Mat rotate(Mat src, double angle);
void perspective_transformation(Mat& input, Mat& output,   RotatedRect minRect);
// Function header
void take_contours(Mat& src, Mat& src_gray, Mat& imgcnt, vector<vector<Point> >& contours, vector<Vec4i>& hierarchy, vector<RotatedRect>& minRect, Mat& test, vector<vector<Point> >& triangles, vector<Vec4i>& hierarchyOfTriangles);
void  take_colors(vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max);
void define_color(vector<string>& clr_definedM1, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max, vector<double> largestSizeM1, Mat& test);
void define_color_using_pixels(vector<string>& clr_definedM2, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max,  vector<double> largestSizeM2, Mat& test);
void comparation(vector<string>& clr_definedM1, vector<string>& clr_definedM2,  int nmb, vector<RotatedRect>& minRect);
static double angle( Point pt1, Point pt2, Point pt0 );

float euclideanDist(Point& p, Point& q);
int nmbMAX(vector<int >& to_analyse);
void analyse(vector<vector<string > >& color_to_an1, vector<vector<string > >& color_to_an2,
 vector<vector<RotatedRect > >& rect_to_an1, vector<vector<RotatedRect > >& rect_to_an2, vector<string >& clr1, vector<string >& clr2, vector<RotatedRect >& rect1, vector<RotatedRect  >& rect2 );

void  findcolor(Mat& src, vector<Mat >& roiJeune, Mat& imgcnt,  vector<vector<Point > >& contoursReturn, vector<RotatedRect >& outputFinalMax,  vector<string >& colorReturn,  vector<vector<string > >& color_to_an1, vector<vector<RotatedRect > >& rect_to_an2, vector<vector<RotatedRect > >& rect_to_an1,  vector<Point2f >& jeuneCenters, vector<vector< Point > >& triangles, vector<string >& names);


void giveme1d( vector<vector<string > >& color_to_an1, vector<vector<RotatedRect > >& rect_to_an1, vector<RotatedRect >& positions1,   vector<string >& clrs1);
