
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

using namespace cv;
using namespace std;

Mat rotate(Mat src, double angle);
void perspective_transformation(Mat& input, Mat& output, vector<vector<Point> >& squares, int nmb, Mat& test);
double find_angle(vector< vector< Point > >, int nmb);
void rotate(Mat& input, Mat& output, vector<vector<Point> >& squares, int nmb);
