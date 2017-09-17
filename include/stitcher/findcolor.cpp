#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <ros/package.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>  
#include <fstream>
//#include <aruco.hpp> 

//#include "transfUNI.h"
using namespace cv;
using namespace std;


#define PI 3.14159265
// Function header
Mat rotate(Mat src, double angle);
void perspective_transformation(Mat& input, Mat& output,   RotatedRect minRect);
void take_contours(Mat& src, Mat& src_gray, Mat& imgcnt, vector<vector<Point> >& contours, vector<Vec4i>& hierarchy, vector<RotatedRect>& minRect, Mat& test, vector<vector<Point> >& triangles, vector<Vec4i>& hierarchyOfTriangles);
void  take_colors(vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max);
void define_color(vector<string>& clr_definedM1, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max, vector<double> largestSizeM1, Mat& test);
void define_color_using_pixels(vector<string>& clr_definedM2, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max,  vector<double> largestSizeM2, Mat& test);
void comparation(vector<string>& clr_definedM1, vector<string>& clr_definedM2,  int nmb, vector<RotatedRect>& minRect);
static double angle( Point pt1, Point pt2, Point pt0 );

float euclideanDist(Point2f& p, Point2f& q) {
    Point diff = p - q;
    return sqrt(diff.x*diff.x + diff.y*diff.y);
}

int nmbMAX(vector<int >& to_analyse){
  if(to_analyse.size() == 0)
    return 0;
  else{
    double max = to_analyse[0];
    int nmb = 0;
    if(to_analyse.size() > 1)
      for(int i = 1; i < to_analyse.size() - 1 ; i++){
        if(max < to_analyse[i])    
	  max = to_analyse[i];
  	  nmb = i;
      }
      return nmb;
    }
}

void giveme1d( vector<vector<string > >& color_to_an1, vector<vector<RotatedRect > >& rect_to_an1, vector<RotatedRect >& positions1,   vector<string >& clrs1){


//here we transform 2d vector of colors and positions to 1d vector
    for(int i = 0; i < color_to_an1.size(); i++){//2
      for(int j = 0; j < color_to_an1[i].size(); j++){ //3
        positions1.push_back(rect_to_an1[i][j]);   
        clrs1.push_back(color_to_an1[i][j]);   
      }//3
    }//2

}

int getColorNmb(string curClr){

  if(curClr == "red")
    return 0;
  if(curClr == "orange")
    return 1;
  if(curClr == "yellow")
    return 2;
  if(curClr == "green")
    return 3;
  if(curClr == "aqua")
    return 4;
  if(curClr == "blue")
    return 5;
  if(curClr == "purple")
    return 6;
  if(curClr == "pink")
    return 7;
  if(curClr == "red")
    return 8;
  if(curClr == "black")
    return 9;
  if(curClr == "white")
    return 10;
  if(curClr == "circle")
    return 11;
}
string color_return(int number){
  if(number == 0)
    return  "red";
  if(number == 1)
    return "orange";
  if(number == 2)
    return "yellow";
  if(number == 3)
    return "green";
  if(number == 4)
    return "aqua";
  if(number == 5)
    return "blue";
  if(number == 6)
    return "purple";
  if(number == 7)
    return "pink";
  if(number == 8)
    return "red";
  if(number == 9)
    return "black";
  if(number == 10)
    return "white";
  if(number == 11)
    return "circle";
}

void analyse(vector<vector<string > >& color_to_an1, vector<vector<string > >& color_to_an2,
 vector<vector<RotatedRect > >& rect_to_an1, vector<vector<RotatedRect > >& rect_to_an2, vector<string >& clr1, vector<string >& clr2, vector<RotatedRect >& rect1, vector<RotatedRect  >& rect2 ){
//initisialisation  

  vector<RotatedRect > positions1;
  vector<string > clrs1;

  if(color_to_an1.size() != 0){//1
    giveme1d(color_to_an1, rect_to_an1, positions1, clrs1);//from 2d vector we get 1d
  }//1 

  int curNmb;
  vector<vector<RotatedRect > >  outputPos1;
  vector<vector<string > >  outputClr1;
  vector<Point2f > labels;

//we sort 1d array and create 2d where every vector is associated with one position
  if(positions1.size() != 0){//1
    for(int f1 = 0; f1 < positions1.size(); f1++){//2
      vector<RotatedRect > current;
      vector<string > currentClr;
      current.push_back(positions1[f1]);
      currentClr.push_back(clrs1[f1]);
      curNmb = 0;
      if(f1 == 0){//3
        outputPos1.push_back(current);
        outputClr1.push_back(currentClr);
        labels.push_back(current[0].center);
      }//3
      else
      for(int f2 = 0; f2 < labels.size(); f2++){//3
	if(euclideanDist(labels[f2], current[0].center) < 20)
	  curNmb = f2;
      }//3
      if(curNmb == 0){
	labels.push_back(positions1[f1].center);
        outputPos1.push_back(current);
        outputClr1.push_back(currentClr);
      }
      else{
        outputPos1[curNmb].push_back(current[0]);
        outputClr1[curNmb].push_back(currentClr[0]);	
      }
    }//2
  }//1

  int colors[10];
  for(int i = 0; i < 10; i++)
    colors[i] = 0;

  int number; string colorRet;
  int umax;

  //We sort here we have found previously
  for(int x = 0; x < outputPos1.size(); x ++){//1
    for(int y = 0; y < outputPos1[x].size(); y ++ ){//2
      number = getColorNmb(outputClr1[x][y]);
      colors[number]++;
    }//2
   for(int u = 0; u < 10; u++){
     if(u == 0)
       umax = 0;
     else 
	if(colors[umax] > colors[u]  )
	  umax = u;
   }
   colorRet = color_return(umax);
   clr1.push_back(colorRet);
   rect1.push_back(outputPos1[x][umax]);
  }//1

//visualisations
/*
  if(outputPos1.size() != 0){
    cout<<"analyse outputPos1 size " << outputPos1.size()<<endl;
    cout<<"analyse outputClr1 size " << outputClr1.size()<<endl;
    for(int i = 0; i < outputPos1.size(); i++){
      for(int j = 0; j < outputPos1[i].size(); j++){
       cout<<"analyse outputPos1 size in every vector " << outputPos1[i].size()<<endl;
       cout<<"analyse outputClr1 size  in every vector" << outputClr1[i].size()<<endl;
      }
    }
  }*/
}


void  findcolor(Mat& src, vector<Mat >& roiJeune, Mat& imgcnt,  vector<vector<Point > >& contoursReturn, vector<RotatedRect >& outputFinal_, vector<string >& colorReturn,  vector<vector<string > >& color_to_an1, vector<vector<RotatedRect > >& rect_to_an2, vector<vector<RotatedRect > >& rect_to_an1, vector<Point2f >& jeuneCenters, vector<vector< Point > >& triangles, vector<string >& names){

  vector<string> clr_names;

  vector<Scalar> clr_min, clr_max;

  Mat test, src_gray,  roi;

  take_colors(clr_names, clr_min, clr_max);
  
  bool trig; int change1, change2;

  vector<string> clr_definedM1, clr_definedM2;
  vector<vector<Point> > contours;

  vector<Vec4i> hierarchy;
  vector<double> largestSizeM1;
  vector<double> largestSizeM2;
  vector<Vec4i> hierarchyOfTriangles;
  vector<RotatedRect> minRect, minRectToWrite1, minRectToWrite2, yellowFields1, yellowFields2;

  take_contours(src, src_gray, imgcnt, contours, hierarchy, minRect, test, triangles, hierarchyOfTriangles);






  vector<vector<RotatedRect> > outputRect2;
  vector<Point2f > labels;
  vector<RotatedRect > outputFinal;
  vector<vector<Point> > outputCnt;
  int curNmb;





//it sorts all contours and creates array of vectors which of them is associated with one place on the image
  if(contours.size() != 0){//1
    for(int f1 = 0; f1 < contours.size(); f1++){//2
      vector<RotatedRect > current;
      current.push_back(minRect[f1]);
      curNmb = 0;
      if(f1 == 0){//3
        outputRect2.push_back(current);
        labels.push_back(current[0].center);
      }//3
      else
      for(int f2 = 0; f2 < labels.size(); f2++){//3
	if(euclideanDist(labels[f2], current[0].center) < 10)
	  curNmb = f2;
      }//3
      if(curNmb == 0){
	labels.push_back(minRect[f1].center);
        outputRect2.push_back(current);
      }
      else
        outputRect2[curNmb].push_back(current[0]);

    }//2

//finding of all the smallest contours around every center of RotatedBox
  int min, minNmb;
  for(int f1 = 0; f1 < outputRect2.size(); f1++ ){// 1-0.5
    for(int f2 = 0; f2 < outputRect2[f1].size(); f2++ ){//2
      if(f2 == 0){//3
	min = outputRect2[f1][f2].size.width;
	minNmb = f2;
      }//3
      else{//3
	if(outputRect2[f1][f2].size.width < min){//4
	  min = outputRect2[f1][f2].size.width;
	  minNmb = f2;	
	}//4
      }//3
    }//2


    outputFinal.push_back(outputRect2[f1][minNmb]);

  }// 1-0.5
//finding of the largest contours around every center of RotatedBox
  int max, maxNmb;
  for(int f1 = 0; f1 < outputRect2.size(); f1++ ){// 1-0.5
    for(int f2 = 0; f2 < outputRect2[f1].size(); f2++ ){//2
      if(f2 == 0){//3
	max = outputRect2[f1][f2].size.width;
	maxNmb = f2;
      }//3
      else{//3
	if(outputRect2[f1][f2].size.width > max){//4
	  max = outputRect2[f1][f2].size.width;
	  maxNmb = f2;	
	}//4
      }//3
    }//2


//    outputFinalMax.push_back(outputRect2[f1][maxNmb]);  // it isn't needed
  }// 1-0.5

// we find colors in the smallest contours, if yes, we will track it futher
  for(int f4 = 0; f4 < outputFinal.size(); f4++){//2
    change2 = clr_definedM2.size();
    change1 = clr_definedM1.size();

    perspective_transformation(src, roi, outputFinal[f4]);
    define_color(clr_definedM1, roi, clr_names, clr_min, clr_max, largestSizeM1, test );
    define_color_using_pixels(clr_definedM2, roi, clr_names, clr_min, clr_max, largestSizeM2, test );

    if( ((clr_definedM2.size() - change2 ) != 0) )
      minRectToWrite2.push_back(outputFinal[f4]);
    if((clr_definedM1.size() - change1 ) != 0)
      minRectToWrite1.push_back(outputFinal[f4]);
  }//2


//visualisation //change minRectToWrite and clr_defined


  for(int vis = 0; vis < minRectToWrite2.size(); vis++){//2
      putText(imgcnt, clr_definedM2[vis], minRectToWrite2[vis].center,  FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255, 0), 1,8, false );
    if((clr_definedM2[vis] == "yellow")||(clr_definedM2[vis] == "orange"))
      yellowFields1.push_back(minRectToWrite1[vis]);
  }//2

  for(int vis = 0; vis < minRectToWrite1.size(); vis++){//2 //
      outputFinal_.push_back(minRectToWrite1[vis]);///////-****************
      names.push_back(clr_definedM1[vis]);
      putText(imgcnt, clr_definedM1[vis], Point(minRectToWrite1[vis].center.x,minRectToWrite1[vis].center.y - 10),  FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0, 255), 1,8, false );
    if((clr_definedM1[vis] == "yellow")||(clr_definedM1[vis] == "orange"))
      yellowFields2.push_back(minRectToWrite1[vis]);
  }//2
  //names = clr_definedM1;

//Creating of a images massive with yellow background  
  Point coor_prec;
  Mat roiY;
  
  for(int j1 = 0; j1 < yellowFields1.size(); j1++ ){//2
    if(j1 == 0){//3
        perspective_transformation(src, roiY, yellowFields1[j1]);
        roiJeune.push_back(roiY);
	jeuneCenters.push_back(yellowFields1[j1].center);
        roiY.release();
      }//3
    else{//3 
      trig = false;
      for(int r = 0; r < jeuneCenters.size(); r++)
	if(euclideanDist(yellowFields1[j1].center,  jeuneCenters[r]) > 100){
          perspective_transformation(src, roiY, yellowFields1[j1]);
          roiJeune.push_back(roiY);
          roiY.release();
        }//4
      }//3
  } //2

  for(int j1 = 0; j1 < yellowFields2.size(); j1++ ){//2
    if((j1 == 0) && (yellowFields1.size() == 0) ){//3
        perspective_transformation(src, roiY, yellowFields2[j1]);
        roiJeune.push_back(roiY);
	jeuneCenters.push_back(yellowFields2[j1].center);
        roiY.release();
    }//3
    trig = false;
    for(int r = 0; r < jeuneCenters.size(); r++)
      if(euclideanDist(yellowFields2[j1].center,  jeuneCenters[r]) > 100){//3

      perspective_transformation(src, roiY, yellowFields2[j1]);
      roiJeune.push_back(roiY);
      roiY.release();
    }//3
  } //2



  contoursReturn = contours;
   



  }//1
  
}

//this function defines rectangles
void take_contours(Mat& src, Mat& src_gray,  Mat& imgcnt, vector<vector<Point> >& contours, vector<Vec4i>& hierarchy, vector<RotatedRect>& minRect, Mat& test, vector<vector<Point> >& triangles, vector<Vec4i>& hierarchyOfTriangles)
{

  Mat canny_output;
  vector<vector<Point> > cur_cnt;
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  vector<Vec4i> hierarchyCur;
  double l;
  int thresh = 40;
  int max_thresh = 100;
  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, max_thresh, 3 );
test = canny_output;
  /// Find contours
  findContours( canny_output, cur_cnt, hierarchyCur, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  //Choosing of the biggest contours
  for(int y = 0; y<cur_cnt.size(); y++){
    l = arcLength(cur_cnt[y], true) ;
    if((l>10)&& (l < 2240)){

///////////////Below we find only rectangles
           vector<Point> approx;
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(cur_cnt[y]), approx, arcLength(Mat(cur_cnt[y]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
//rectangles finding
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) && fabs(contourArea(Mat(approx))) < 100000)
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 ){

                          contours.push_back(approx);
			  hierarchy.push_back(hierarchyCur[y]);			

		     }
                }
//triangles finding
		if( approx.size() == 3 &&
                    fabs(contourArea(Mat(approx))) > 100 &&
                    isContourConvex(Mat(approx)) && fabs(contourArea(Mat(approx))) < 100000){
			double maxCosineT = 0;

			for( int j1 = 2; j1 < 5; j1++ ){
                        // find the maximum cosine of the angle between joint edges
                          double cosineT = fabs(angle(approx[j1%3], approx[j1-2], approx[(j1-1)%3]));
                          maxCosineT = MAX(maxCosineT, cosineT);
                        }
	                if(( maxCosineT < 0.7 )&&( maxCosineT > 0.3)){
	  		  triangles.push_back(approx);
			  hierarchyOfTriangles.push_back(hierarchyCur[y]);
			}
		}

    }
  }
  /// Draw contours
  imgcnt = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
  {
      Scalar color = Scalar( 0, 0, 255 );
      drawContours( imgcnt, contours, i, color, 2, 8, hierarchy, 0, Point() ); 
  }

  for(int j = 0; j < triangles.size(); j++){
      Scalar colorT = Scalar( 255, 0, 255 );
      drawContours( imgcnt, triangles, j, colorT, 2, 8, hierarchyOfTriangles, 0, Point() ); 
  }

  for( int t = 0; t < contours.size(); t++ )
    minRect.push_back( minAreaRect( Mat(contours[t])) );
}

void  take_colors(vector<string>& clr_names, vector<Scalar>& clr_min,  vector<Scalar>& clr_max){

  string color_file_name =  ros::package::getPath ("stitcher") + "/include/stitcher/colors.txt";

  ifstream inputFile;
  inputFile.open (color_file_name.c_str(), std::ifstream::in);
  string line, name;
  int var1, var2, var3, var4, var5, var6;
  while (getline(inputFile, line))  {
   istringstream ss(line);
   ss >> name >> var1 >> var2 >> var3 >> var4 >> var5 >> var6;
   clr_names.push_back(name);
   clr_min.push_back( Scalar(var1, var2, var3) );
   clr_max.push_back( Scalar(var4, var5, var6) );
  }

}


static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}



void define_color(vector<string>& clr_definedM1, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max,  vector<double> largestSizeM1, Mat& test){
  Mat tresholded, imgHSV;
  int maxNMB, contourArea;
  vector<int > contour_area;
  vector<vector<Point> > cnts;
  vector<int > NMBS;
  vector<int > areas;
  vector<Vec4i> hrs;
  bool trig;
cout<< " we are in define_color " <<" roi.type() " << roi.type() << endl;
  if(roi.type() == 16){ // with no reasons it gives sometimes 5 instead of 16, this condition protects the algorithme

cout<<" DEFINE_COLOR "<< clr_names.size() <<endl;
//double(contourArea / imgHSV.cols * imgHSV.rows )<<
  cvtColor(roi, imgHSV, CV_BGR2HSV, 3);
  for(int j = 0; j < clr_names.size(); j++){
     contourArea = 0;
     trig = false;
     inRange(imgHSV, clr_min[j], clr_max[j], tresholded);
     findContours( tresholded, cnts, hrs, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

     //moments
     vector<Moments> mu(cnts.size() );
     for( size_t i = 0; i < cnts.size(); i++ ){
       mu[i] = moments( cnts[i], false );
       contourArea += mu[i].m00 ;
     }

     
     if((contourArea > 0.41 * imgHSV.cols * imgHSV.rows)&&(!trig)){
	clr_definedM1.push_back(clr_names[j]);
        trig = true;	}
//       contour_area.push_back(contourArea);
   }
  }

}

/*void define_color(vector<string>& clr_definedM1, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max,  vector<double> largestSizeM1, Mat& test){//fnc
  Mat tresholded, imgHSV, src_gray;
  int maxNMB, contourArea;
cout<<"WE ARE IN DEFINE_COLOR" << endl;
  vector<int > contour_area;
  vector<vector<Point> > cnts;
  vector<int > NMBS;
  vector<int > areas;
  vector<Vec4i> hrs;
  bool trig;
  int maxArea, maxArea2 = 0, maxNmb;
  if(roi.type() == 16){ // with no reasons it gives sometimes 5 instead of 16, this condition protects the algorithme //ifGrand
  cvtColor(roi, imgHSV, CV_BGR2HSV, 3);

  for(int j = 0; j < clr_names.size(); j++){//for_j
     contourArea = 0;
     maxArea = 0;
     inRange(imgHSV, clr_min[j], clr_max[j], tresholded);
     findContours( tresholded, cnts, hrs, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
     trig= false;
     //moments
     vector<Moments> mu(cnts.size() );
     for( size_t i = 0; i < cnts.size(); i++ ){//for_i
       mu[i] = moments( cnts[i], false );
       contourArea += mu[i].m00 ;
     }//for_i
    
     if((contourArea > 0.41 * imgHSV.cols * imgHSV.rows)){ //if2

	 if(trig == false){
	   maxArea = contourArea;	
	   trig = true; 
	   maxNmb = j;
	  }
	 else{
	   if(contourArea > maxArea)
	     maxArea = contourArea; 
	 }


     } //if2
    
    if(maxArea != 0){//if3

      if(maxArea2 == 0){
        maxArea2 = maxArea;
	maxNmb = j;
      }
      else{
	if(maxArea > maxArea2){
	  maxArea2 = maxArea;
	  maxNmb = j;
	}
      }

    }//if3
    

  }//for_j

  if(maxNmb == 5){ //if4
    cvtColor( roi, src_gray, CV_BGR2GRAY );
    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

    if(circles.size()!=0)	
      maxNmb = 10;

  }//if4

  if(maxArea2 != 0)
	clr_definedM1.push_back(clr_names[maxNmb]);

 }//ifGrand

 cout<<"IM IN DEFINE_COLOR, RESULT (number of colors already) IS "<< clr_definedM1.size() <<endl;


}//fnc*/

/*  for(int j = 0; j < clr_names.size(); j++){
     contourArea = 0;
     trig = false;
     inRange(imgHSV, clr_min[j], clr_max[j], tresholded);
     findContours( tresholded, cnts, hrs, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

     //moments
     vector<Moments> mu(cnts.size() );
     for( size_t i = 0; i < cnts.size(); i++ ){
       mu[i] = moments( cnts[i], false );
       contourArea += mu[i].m00 ;
     }
     if((contourArea > 0.41 * imgHSV.cols * imgHSV.rows)&&(!trig)){
	clr_definedM1.push_back(clr_names[j]);
        trig = true;	}
//       contour_area.push_back(contourArea);
   }
  }*/

void define_color_using_pixels(vector<string>& clr_definedM2, Mat& roi,   vector<string>& clr_names, vector<Scalar>& clr_min, vector<Scalar>& clr_max,  vector<double> largestSizeM2, Mat& test){
  Mat imgHSV;
  int pxl_cnt, H, S, V, x_cur, y_cur, x_0, y_0 = 18;
  int delta = 20; //imgHSV.cols/(0.05 * imgHSV.cols)
  if((roi.type() == 16)&&(roi.depth() == 0)){
    cvtColor(roi, imgHSV, CV_BGR2HSV);
     for(int j = 0; j < clr_names.size(); j++){
       pxl_cnt = 0;

       for(int y_nmb = 0; y_nmb < int(0.05*imgHSV.rows); y_nmb ++){
         for(int x_nmb = 0; x_nmb < int(0.05*imgHSV.cols); x_nmb ++){
	   if((y_nmb % 2) == 1)
	     x_0 = 0;
	   else
	     x_0 = 10;
	   x_cur = - x_0 + x_nmb*delta; // -x_0 is used to start from 10 instead of 20
	   y_cur = -y_0 + y_nmb*delta; //-y_0 is used to start from 2 instead of 20
	   Vec3b hsv=imgHSV.at<Vec3b>(Point(x_cur, y_cur)); //acces to one pixel
	   H=hsv.val[0]; //hue
	   S=hsv.val[1]; //saturation
	   V=hsv.val[2]; //value
	   if( ( H > clr_min[j][0] )&&(H < clr_max[j][0]))
             if( ( S > clr_min[j][1] )&&(S < clr_max[j][1]))
               if( ( V > clr_min[j][2] )&&(V < clr_max[j][2]))
		 pxl_cnt++;
	}//x
      }//y
    if(pxl_cnt > 0.5 * (0.05*imgHSV.rows) * (0.05*imgHSV.cols))
	clr_definedM2.push_back(clr_names[j]);
    }//for clr_names
  }//if
}

void comparation(vector<string>& clr_definedM1, vector<string>& clr_definedM2, int nmb, vector<RotatedRect>& minRect){
  cout<<"CONTOUR NMB: "<<nmb<<endl;
//  cout<<"SizeM1: "<<clr_definedM1.size()<<endl;
//   cout<<"Position "<<  minRect[nmb].center <<endl;
  for(int i = 0; i < clr_definedM1.size(); i++)
	cout<< i <<" Color of methode I is "<<clr_definedM1[i]<<endl;
//  cout<<"SizeM2: "<<clr_definedM2.size()<<endl;
  for(int j = 0; j< clr_definedM2.size(); j++)
	cout<< j <<" Color of methode II is "<<clr_definedM1[j]<<endl;

}

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}


void perspective_transformation(Mat& input, Mat& output, RotatedRect minRect){
    double width, height;
    Point2f square[4];
    // Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4]; 
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
         
    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    //Input and Output Image;
   // Mat input, output;
     
    //Load the image
   // input = imread( "lena.jpg", 1 );
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );
 
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input 

/*    vector<Moments> mu(squares.size() );
    for( size_t i = 0; i < squares.size(); i++ )
      { mu[i] = moments( squares[i], false ); }

*/


    minRect.points(square);  
    for(int i = 0; i<4; i++) {
      if((square[i].x < minRect.center.x)&&(square[i].y < minRect.center.y))
	inputQuad[0] = square[i];
      if((square[i].x > minRect.center.x)&&(square[i].y < minRect.center.y))
	inputQuad[1] = square[i];
      if((square[i].x > minRect.center.x)&&(square[i].y > minRect.center.y))
	inputQuad[2] = square[i];
      if((square[i].x < minRect.center.x)&&(square[i].y > minRect.center.y))
	inputQuad[3] = square[i];
    }
    width = norm(Mat(inputQuad[0]), Mat(inputQuad[1]));
    height =  norm(Mat(inputQuad[1]), Mat(inputQuad[2]));
	  //width = abs(square[1].x - square[0].x);
	  //height =  abs(square[0].y - square[3].y); 
//cout<<width<<" "<< height<<endl;
    outputQuad[0] = Point2f( 0, 0 );
    outputQuad[1] = Point2f( width, 0 );
    outputQuad[2] = Point2f( width, height );
    outputQuad[3] = Point2f( 0, height  );


    // The 4 points where the mapping is to be done , from top-left in clockwise order

    // Get the Perspective Transform Matrix i.e. lambda 
    lambda = getPerspectiveTransform( inputQuad, outputQuad);
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,input.size());

    //If the picture is vertical, we make her horizontal

    Point min, max;
    min.x = 5; min.y = 5;
    max.x = width-5; max.y = height-5;

    if((max.x < output.cols)&&(max.y < output.rows))
      if((max.x > min.x)&&(max.y > min.y)) 
      output = output(Rect(min, max));
    else 
      output = Mat(0, 0, CV_32FC1);
    //If the picture is vertical, we make her horizontal
  
}
