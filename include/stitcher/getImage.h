#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>
//#include "opencv2/text.hpp"
//#include "opencv2/text/erfilter.hpp"
//#include "opencv2/text/ocr.hpp"
#include<opencv2/ml/ml.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <cstdlib>
#include <time.h> 
#include <vector>  
#include <fstream>



using namespace cv::ml;
using namespace cv;
using namespace std;

void wait ( int seconds );
Mat binarisation(Mat& image, int color);
void bounds(Mat &image);
vector<Point>    getrect(vector< vector<Point> > & contours);
vector<Mat> getImg(Mat& image, int color,Mat& conts, 	vector<int >& positions, Mat& digitsImg);//, int Case);

//int getImgMain();
