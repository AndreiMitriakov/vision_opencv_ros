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

#include "getImage.h"

using namespace cv::ml;
using namespace cv;
using namespace std;

RNG rng(12343);

void wait ( int seconds ) 
{ 
  clock_t endwait; 
  endwait = clock () + seconds * CLOCKS_PER_SEC ; 
  while (clock() < endwait) {} 
} 

Mat binarisation(Mat& image, int color);//, int Case);

void bounds(Mat& img_to_recog){
//black bounds adding
   Mat largerImage = Mat::zeros(Size(20,20), img_to_recog.type());
   img_to_recog.copyTo(largerImage(Rect(3,3,img_to_recog.cols,img_to_recog.rows)));
   img_to_recog = largerImage;
	
}

vector<Mat> getImg(Mat& image, int color, Mat& conts, 	vector<int >& positions){//, int Case){

	Size size(14, 14);
	vector<Mat> digits;
	vector<Mat> cnts;
	int nmb = 2;
	int type;
	Mat imgbin, img_one_cnt, img_to_recog;
	
        imgbin = binarisation(image, color);

	//resize imgbin - deleting of border
	int paramY;
	int borderX = int( imgbin.cols / 20);// borders cutting
//	cout<<"imgbin.cols / imgbin.rows "<<double(imgbin.cols / imgbin.rows)<<endl;
        if(double(imgbin.cols / imgbin.rows) > 2)
	  paramY = 2;
	else 
	  paramY = 1;
	int borderY = int( imgbin.rows / (paramY * 20) );// borders cutting
	Mat cur = imgbin(Rect( Point(borderX,borderY), Point(imgbin.cols - borderX, imgbin.rows - borderY) ) );
	imgbin = cur;

	conts = cur;

	vector< vector<Point> > contours, contoursBig, contoursOnLinePre;
	vector<Vec4i> hierarchy; 
	findContours( imgbin, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Moments> mu(contours.size() );	


	for( int j = 0; j < contours.size(); j++ )
          mu[j] = moments( contours[j], false ); 

	//in this part it sorts all contours to find several needed contours
	//10 height of one digit, 5 - width, 30 height of the card, 40 its width
        //0.0416666 = 10 * 5 /(30 * 40) 
	//but for one part of the card we should use 0.08
	double surface;
	for( int j = 0; j < contours.size(); j++ ){
	  surface = double( mu[j].m00 / (imgbin.cols * imgbin.rows) );
//	  cout << "surface relative " <<surface <<endl;
//	  cout << "mu[j].m00  " <<mu[j].m00 <<endl;
//	  cout << "imgbin.cols * imgbin.rows  " <<imgbin.cols * imgbin.rows <<endl;

	  if( ( surface > 0.01 )  && (surface < 0.2)) //0.04 is just for pre-experiments
	    contoursBig.push_back(contours[j]); //we found big contours
	  }
	//next we should delete all contours which aren't on the central line
	vector<Point > rect_cntrs; int x_cur, y_cur; 
	Point cur_cntr;
	vector<Point > rect_cntrsPre;

	for( int i = 0; i < contoursBig.size(); i++ ){//creation of associated vector with rects centers 
	  cur_cntr = Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00);
	  rect_cntrs.push_back(cur_cntr);
	}
	int y_average;
//cout << "contoursBig "<< contoursBig.size()<<endl;
	for(int i = 0; i < contoursBig.size(); i++){
	  if(abs(rect_cntrs[i].y-int(imgbin.rows * 0.5) ) < 30 ){//on the central line
	    if((rect_cntrs[i].x > 10) && (rect_cntrs[i].x < (imgbin.cols-10) ) ){//marge from lef and right

                  contoursOnLinePre.push_back(contoursBig[i]);
                  rect_cntrsPre.push_back(rect_cntrs[i]);
	    }
	  }
	}

//cout << " HERE  "<<endl;


//organissing of all contours in correct array, where the minimal is on the first place and so on
//cout << "contoursOnLinePre "<< contoursOnLinePre.size()<<endl;	

//for(int ex = 0; ex < rect_cntrsPre.size(); ex++ )	
  //cout << "rect_cntrsPre.x "<< ex<<" Pos: "<< rect_cntrsPre[ex].x<<endl;	


	vector< vector<Point> > contoursOnLine(contoursOnLinePre.size());
	vector<Point > rect_cntrsOnLine(rect_cntrsPre.size());
	vector<int > pos(rect_cntrsPre.size());
	
	contoursOnLine = contoursOnLinePre;
	rect_cntrsOnLine = rect_cntrsPre;
	
	for(int i = 0; i < rect_cntrsPre.size(); i++)
	  pos[i] = rect_cntrsOnLine[i].x;	
	
	positions = pos;
	
/*	vector<Point > cur_contour;
	int trigPos = 0;
	for(int i = 0; i < contoursOnLinePre.size(); i++ ){
	 
            cur_contour = contoursOnLinePre[0];
	    cur_cntr = rect_cntrsPre[0];   
	  
	  for(int j = 0; j < contoursOnLinePre.size(); j++ ){
	    if((cur_cntr.x > rect_cntrsPre[j].x) && ( rect_cntrsPre[j].x > trigPos ) ){
	      cur_cntr = rect_cntrsPre[j];
	      cur_contour = contoursOnLinePre[j];
	    }
	  
	  }
          contoursOnLine[i] = cur_contour;
	  rect_cntrsPre[i] = cur_cntr;
	  trigPos = cur_cntr.x;

	}*/

//for(int ex = 0; ex < rect_cntrsPre.size(); ex++ )	
 // cout << "rect_cntrs.x "<< ex<<" Pos: "<< rect_cntrs[ex].x<<endl;	
	//  cur_contour = contoursOnLinePre[i];

	//arrrange contoursOnLinePre into contoursOnLine 


	 

//cout << "contoursOnLine "<< rect_cntrsOnLine.size()<<endl;
//for(int ex = 0; ex < rect_cntrsOnLine.size(); ex++ )	
 // cout << "rect_cntrsOnLine "<< rect_cntrsOnLine[ex].x<<endl;	



	//creating of bounding boxes around all contours
        vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect > boundRect( contours.size() );
	for( int i = 0; i < contoursOnLine.size(); i++ ){
	  approxPolyDP( Mat(contoursOnLine[i]), contours_poly[i], 3, true );
	  boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	}

	//we add digit images to one vector 
	Point A, B;
	int ymax, ymin, xmax, xmin; 
	Mat dst, fnl, src;
	for(int i =0; i < contoursOnLine.size(); i++){	
	  //Here we choose two vertices
	//cout<<"boundRect "<< boundRect.size()<<endl;
	  ymin = boundRect[i].y; xmin = boundRect[i].x; 	  
	  ymax = boundRect[i].y + boundRect[i].height; xmax = boundRect[i].x + boundRect[i].width; 
	  A = Point(xmin, ymin);
	  B = Point(xmax, ymax);
	  src = imgbin( Rect(A, B)); 
	  resize(src, dst, size );

	  bounds(dst);
	  digits.push_back(dst);
	}

	//Next, it draws all found contours
    /*    Mat drawing;
        drawing = Mat::zeros( image.size(), CV_8UC3 );

	for( int i = 0; i<contours.size() ; i++ )
        {
          Scalar color = Scalar( rng.uniform(0, 0), rng.uniform(0,233), rng.uniform(0,0) );
          drawContours( drawing, contoursBig, i, color, 1, 1, hierarchy, 0, Point() );
	}*/

//cout<<"DIGITS "<<digits.size()<<endl;*/
/*	  int n = digits.size();
          if(n == 5 ){
	    conts = digits[0];
cout<<"getImg n!=0"<<endl;
	  }
          else{
	    conts = cur;
cout<<"getImg; n==0; conts = imgbin"<<endl;
	  }*/
//conts = cur;

//cout<<"Size of rectangle with digits " << conts.cols<<" "<<conts.rows << endl;

//	  cout<<"End of getImg, size of digits: "<< digits.size()<<endl;
	  return digits;  

		

}


Mat binarisation(Mat& image, int color)//, int Case)
{
        //int iLowH, iHighH, iLowS, iHighS, iLowV, iHighV; 
	/*if(color == 1){*/
	 
	  int iLowH =0;
	  int iHighH = 256;

	  int iLowS = 50; 
  	  int iHighS = 256;

	  int iLowV = 1;

	  int iHighV = 120; //}

	/*if(color == 0){
	  int iLowH = 0;
	  int iHighH = 233;

	  int iLowS = 0; 
	  int iHighS = 233;

	  int iLowV = 80;
	  int iHighV = 233;
	}*/

	int edgeThresh = 12;
	int lowThreshold = 10;
	int const max_lowThreshold = 100;
	int ratio = 5;
	int kernel_size = 3;
	Mat dst_gray, dst;
	cvtColor(image, dst_gray, CV_BGR2GRAY);
	blur(dst_gray,  dst, Size(3,3));
        Canny( dst_gray, dst, lowThreshold, lowThreshold*ratio, kernel_size );
	//Treshold
	Mat imgHSV;
	cvtColor(image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	Mat imgThresholded;
	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
  	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) ); 

//morphological closing (removes small holes from the foreground)
	  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) ); 
	  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
	return imgThresholded;
}

