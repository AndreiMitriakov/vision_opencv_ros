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
  

  
void videoOCR(Mat& src, Mat& dist, Mat& cnt, vector<vector<double > >& coords_rect, vector<string >& names, vector<int >& resultOCR)
{
    //initialisation
	
  //  VideoCapture cap(0); // open the video camera no. 0
  //  cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
 //   cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
   /* namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    moveWindow("MyVideo", 1200, 50);
    namedWindow("roi_digit",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    moveWindow("roi_digit", 1200, 650);
    namedWindow("Contours",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    moveWindow("Contours", 600, 50);*/
    	Mat conts, test, rotated_img, imgCNT, res =src ;
        Mat image = src; 
    	vector<vector<Point > > contours;
    	vector<vector<string > >  color_to_an1;

    	vector<vector<RotatedRect > > rect_to_an2, rect_to_an1;
    	vector<string > clr_prev;
    	vector<RotatedRect > minRec_prev;
        vector<vector<double > > crdsTriang;
	vector<int > positions;
        vector<Mat > roiJeune;
	vector<Point2f > jeuneCenters;
//        vector<RotatedRect > boxes;
        vector<vector<Point > > contoursReturn;
	vector<vector< Point > > triangles;
        vector<string > colorReturn;
 	//cap.read(image); 
	vector<RotatedRect > outputFinalMax;
//add initialisation of drawing every 10 times  clr1 and minRec1 in findcolor should be taken every 10 times and drawn futher 10 times

        findcolor(image,  roiJeune, imgCNT,  contoursReturn, outputFinalMax,  colorReturn,  color_to_an1, rect_to_an2,  rect_to_an1,  jeuneCenters,  triangles, names );
//	cout<<"outputFinalMax " << outputFinalMax.size()<<endl;
//	cout<<"colorReturn " << colorReturn.size()<<endl;
//finddistance  doesn't work
        for(int j = 0; j < outputFinalMax.size(); j++ ){
	  vector<RotatedRect> oneRect;//( 1 );
	  oneRect.push_back(outputFinalMax[j]);
  //        cout<<"IM HERE 1"<<endl;
          vector<vector<double > >  crds;
	  finddistance(image, res, oneRect, false, crds);
          coords_rect.push_back(crds[0]);
     //     if(colorReturn.size() != 0)
 	//    if(colorReturn.size() > j)           
          //  names.push_back(colorReturn[j]); // it can create problems if colorReturn.size() != outputFinalMax.size() 
        }

	if(triangles.size() != 0){
	  names.push_back("triangle");
          findDistTrian(res, triangles, crdsTriang);
	  coords_rect.push_back(crdsTriang[0]);
	}
	
cout<<"-------Start---------"<<endl;
//cout<<"Number of yellow images "<<roiJeune.size() <<endl;
	if(roiJeune.size() != 0)
	  for(int i = 0; i < roiJeune.size(); i++){
            resultOCR.push_back( OCR(roiJeune[i], conts, positions) );
            if(resultOCR[0] != -999)
	      cout<<"DIGITS : "<<resultOCR[0]<<" from yellow image nmb "<< i << endl;
	    else	
	      cout<<"NOTHING is detected"<<endl;	
 	  }
cout<<"-------Finish---------"<<endl;
//cout<<"res size"<< res.size()<<endl;
//cout<<"imgCNT size"<< imgCNT.size()<<endl;
        if(outputFinalMax.size() != 0)
          dist = res;
	else
	  dist = res;

	cnt = imgCNT;	



}
//#endif
