
#define USE_OPENCV_3



#ifdef USE_OPENCV_3
   #include <iostream>
   #include <opencv2/highgui.hpp>
   #include <opencv2/imgproc.hpp>
   #include "opencv2/objdetect.hpp"
   #include <opencv2/ml.hpp>
#endif

#ifdef USE_OPENCV_2
   #include <cv.h>
   #include <highgui.h>
   #include <opencv2/ml/ml.hpp>
#endif

#include <iostream>

//#include "getImage.h"

#ifdef USE_OPENCV_3
using namespace cv::ml;
#endif
using namespace cv;
using namespace std;



//HOGDescriptor hog;


void wait ( int seconds );
Mat binarisation(Mat& image, int color);
void bounds(Mat &image);
vector<Point>    getrect(vector< vector<Point> > & contours);
vector<Mat> getImg(Mat& image, int color,Mat& conts, 	vector<int >& positions);//, int Case);


void getSVMParams(SVM *svm);


int SVMevaluate(Mat &testResponse,float &count, float &accuracy,vector<int> &testLabels);

void loadTrainTestLabel2(vector<Mat> &testCells, Mat &image, Mat &conts, vector<int >& positions);


void CreateTrainTestHOG2( vector<vector<float> > &testHOG, vector<Mat> &deskewedtestCells);
void ConvertVectortoMatrix2( vector<vector<float> > &testHOG, Mat &testMat);

void SVMtrain2(Mat &testResponse,Mat &testMat);

int OCR(Mat &image, Mat &conts, vector<int >& positions);
