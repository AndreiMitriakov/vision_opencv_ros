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
  
//#define find_h
  
using namespace cv;
using namespace std;
  
//int thresh = 50, N = 11;
  

  
void videoOCR(Mat& image, vector<vector<double > >& coords_rect, vector<vector<string > >& names)
{
    //initialisation
    int resultOCR;
    VideoCapture cap(0); // open the video camera no. 0
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    moveWindow("MyVideo", 1200, 50);
    namedWindow("roi_digit",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    moveWindow("roi_digit", 1200, 650);

    namedWindow("Contours",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    moveWindow("Contours", 600, 50);
    Mat image,  conts, test, rotated_img, imgCNT; 
    vector<vector<Point > > contours;
    vector<vector<string > >  color_to_an1;

    vector<vector<RotatedRect > > rect_to_an2, rect_to_an1;
    vector<string > clr_prev;
    vector<RotatedRect > minRec_prev;
    int counter = 0;

    while(1){
  //      vector< vector < double >  crds;
    //    vector< vector < double > crdsTriang;
 	counter++;
	vector<int > positions;
        vector<Mat > roiJeune;
	vector<Point2f > jeuneCenters;
        vector<RotatedRect > boxes;
        vector<vector<Point > > contoursReturn;
	vector<vector< Point > > triangles;
        vector<string > colorReturn;
 	cap.read(image); 
	vector<RotatedRect > outputFinalMax;
//add initialisation of drawing every 10 times  clr1 and minRec1 in findcolor should be taken every 10 times and drawn futher 10 times

        findcolor(image,  roiJeune, imgCNT,  contoursReturn, outputFinalMax,  colorReturn,  color_to_an1, rect_to_an2,  rect_to_an1,  jeuneCenters,  triangles );	

	finddistance(image, imgCNT, boxes, false/*, crds*/);
	if(triangles.size() != 0){
          findDistTrian(triangles/*, crdsTriang*/);
	}
cout<<"-------Start---------"<<endl;
//cout<<"Number of yellow images "<<roiJeune.size() <<endl;
	if(roiJeune.size() != 0)
	  for(int i = 0; i < roiJeune.size(); i++){
            resultOCR = OCR(roiJeune[i], conts, positions);
            if(resultOCR != -999)
	      cout<<"DIGITS : "<<resultOCR<<" from yellow image nmb "<< i << endl;
	    else	
	      cout<<"NOTHING is detected"<<endl;	
 	  }
cout<<"-------Finish---------"<<endl;
	analyseOfYellowImages( imgCNT, roiJeune, jeuneCenters);
	//roi showing
	if(roiJeune.size() != 0){
          if(roiJeune[0].rows * roiJeune[0].cols != 0)	
	        imshow("roi_digit", roiJeune[0]);		//conts
        }

	//image showing
        imshow("MyVideo", image);

        //contours showing
	if(imgCNT.cols * imgCNT.rows != 0)
	        imshow("Contours", imgCNT);

	//Escape - ESC
        if (waitKey(30) == 27)  break;        
    }
    return 0;

}
//#endif
