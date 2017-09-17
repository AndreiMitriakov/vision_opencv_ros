#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>  
 #include <sstream> 
#include <fstream>
//#include <aruco.hpp> 
#include <math.h>  
//#include "transfUNI.h"
using namespace cv;
using namespace std;



// Function header



void  finddistance(Mat& image, Mat& imgRes, vector<RotatedRect >& boxes, bool valve, vector< vector < double > >&  crds); // true = valve; false = rectangles
double angle_cos( Point pt1, Point pt2, Point pt0 );
float euclideanDist1(Point2f& p, Point2f& q);
void findDistTrian(Mat& src, vector<vector< Point > >& triangles, vector< vector<double > >& crdsTriang);
