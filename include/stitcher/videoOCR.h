//#ifndef find_h

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
  
#include "findcolor.h"
#include "finddistance.h"
#include "getImage.h"
#include "digit_recog.h"


  
using namespace cv;
using namespace std;
  


void videoOCR(Mat& image, Mat& dist, Mat& cnt, vector<vector<double > >& coords_rect, vector<string >& names, vector<int >& resultOCR);
//#endif
