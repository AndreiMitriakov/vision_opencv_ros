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
using namespace cv;
using namespace std;

void getOrientation(Mat& src,  vector<vector<Point> >& oneCnt, double& angle);

void getDistance(Mat& src, Mat& dist, vector<vector<Point> >& oneCnt, vector<vector<double > >& crds);

void valve(Mat& src, Mat& dist, vector<vector<double > >& coords);
