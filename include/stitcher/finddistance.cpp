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

using namespace cv;
using namespace std;

#define PI 3.14159265


// Function header

void getDistances(Point2f centerOfbox, double size1, double size2,   vector<double >& answers, bool valve, bool triangle);
void drawInfo(RotatedRect box, vector<double >& answers, Mat& src);
float euclideanDist1(Point2f& p, Point2f& q) {
    Point diff = p - q;
    return sqrt(diff.x*diff.x + diff.y*diff.y);
}

double angle_cos( Point2f A0, Point2f A3 )
{// a  - AB, b - BO, c - OA
 double tang = (A0.y - A3.y) / (A3.x - A0.x);   
 double res = atan (tang) * 180.0 / PI;
 return res;
}


void findDistTrian(Mat& src, vector<vector< Point > >& triangles, vector<vector<double > > & crdsTriang){
    vector<RotatedRect > boxT;
    for( int t = 0; t < triangles.size(); t++ )
      boxT.push_back( minAreaRect( Mat(triangles[t])) );
    RotatedRect largest;
    for(int i = 0; i < boxT.size(); i++){
      if(i == 0)
	largest = boxT[0];
      else{
	if(largest.size.height < boxT[i].size.height)
	  largest = boxT[i];
      }
    }
   vector<double > crdsTriangAnswer;
   getDistances(largest.center, largest.size.height, largest.size.height, crdsTriangAnswer, false, true);
   crdsTriang.push_back(crdsTriangAnswer);
 //  if(crdsTriangAnswer.size() != 0)
 //    cout<<" size of triangles " << crdsTriangAnswer[0]<<endl;
   drawInfo(boxT[0], crdsTriangAnswer, src);
}


void drawInfo(RotatedRect box, vector<double >& answers, Mat& src){
  Point2f txtPos;
  stringstream s; string tobe, towrite;
  txtPos.y = box.center.y;
  txtPos.x = box.center.x + box.size.width;
  Scalar  color = Scalar(255, 0, 0);  

  Point2f rect_points[4]; box.points( rect_points );
  for( int j = 0; j < 4; j++ )
    line( src, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );  

  s << int(answers[0]);
  tobe = s.str();
  towrite = "X: " + tobe;
  putText(src, towrite, txtPos,  FONT_HERSHEY_SIMPLEX, 0.4,  Scalar(0, 0, 255), 1,8, false );
  s.str("");

  s << int(answers[1]);
  tobe = s.str();
  towrite = "Y: " + tobe;
  txtPos = Point2f(txtPos.x, txtPos.y + 13 );
  putText(src, towrite, txtPos,  FONT_HERSHEY_SIMPLEX, 0.4,  Scalar(0, 0, 255), 1,8, false );
  s.str("");

  s << int(answers[2]);
  tobe = s.str();
  towrite = "Z: " + tobe;
  txtPos = Point2f(txtPos.x, txtPos.y + 13 );
  putText(src, towrite, txtPos,  FONT_HERSHEY_SIMPLEX, 0.4,  Scalar(0, 0, 255), 1,8, false );
  s.str("");

  s << int(answers[3]);
  tobe = s.str();
  towrite = "L: " + tobe;
  txtPos = Point2f(txtPos.x, txtPos.y + 13 );
  putText(src, towrite, txtPos,  FONT_HERSHEY_SIMPLEX, 0.4,  Scalar(0, 0, 255), 1,8, false );

//  s << int(size2);
//  tobe1 = s1.str();
//  s.str("");
}
//under the question
void getDistances(Point2f centerOfbox, double size1, double size2,   vector<double >& answers, bool valve, bool triangle){
  double size;
  if(valve == true)
    size = 58;
  else{
    if(triangle)
      size = 400;
    else
      size = 297;
  }
  Point2f center = Point(320, 240);
  Point2f difXY = centerOfbox  - center;
  double h = 36; // in pxls, it is the size of the vertical border of interesting
  double calibDist = 125;  
  double pixCal = size * (h / 50);
  double distAbs1 = calibDist * pixCal / size1 ; //125 high of calibration, 50 the border size  during the calibration
  double distAbs2 = pixCal * calibDist / size2; // (125 /  ( 200 * h / 50) ) * size2;
  double distAbs = (distAbs1 + distAbs2) / 2;
  double X = difXY.x * 50 / size2;
  answers.push_back(X);
  double Y = difXY.y * 50 / size2;
  answers.push_back(Y);
  double Z = sqrt(distAbs*distAbs - X*X - Y * Y);
  answers.push_back(Z);
  answers.push_back(distAbs);
}

void  finddistance(Mat& image, Mat& imgRes,  vector<RotatedRect >& boxes, bool valve, vector<vector <double > >& crds)
{
  Mat src = image;
  double size1, size2;
  string tobe, tobe1, tobe2;
  Point sup;
  char *intStr ;
  double res;
  
  Point2f vertices[4];
  for(int a1 = 0; a1 < boxes.size(); a1 ++)
  { 

   boxes[a1].points(vertices);
  /*  for(int a2 = 0; a2 < 4; a2 ++){
      circle(src, vertices[a2], 2 , Scalar(0, 255, 0), -1, 8, 0);
      stringstream ss;  
      ss << a2;
      tobe = ss.str();      
      putText(src, tobe, vertices[a2],  FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,255, 0), 1,8, false );
      ss.str("");
   }*/

   Point2f centerOfbox;
   centerOfbox.x = (vertices[0].x + vertices[2].x)/2;
   centerOfbox.y = (vertices[0].y + vertices[1].y)/2;
   Point2f vertical1[2], vertical2[2];
   if(vertices[0].x > vertices[2].x){
     vertical1[0] = vertices[1];
     vertical1[1] = vertices[2];
     vertical2[0] = vertices[0];
     vertical2[1] = vertices[3];

   }
   else{ 
     vertical1[0] = vertices[0];
     vertical1[1] = vertices[1];
     vertical2[0] = vertices[3];
     vertical2[1] = vertices[2];     

   }
   if(valve){
     size1 = euclideanDist1(vertical1[0], vertical2[0]);
     size2 = euclideanDist1(vertical1[1], vertical2[1]);
   }
   else{
     size1 = euclideanDist1(vertical1[0], vertical1[1]);
     size2 = euclideanDist1(vertical2[0], vertical2[1]);
   }
//comparation
//  cout<<"size1 "<< size1 <<" size2 "<< size2 <<" height "<< boxes[a1].size.width << endl;
   Point2f pos1, pos2;
   stringstream s1, s2;
   pos1.x = (vertical1[0].x + vertical1[1].x)/2;
   pos1.y = (vertical1[0].y + vertical1[1].y)/2;
   //pos2.x = (vertical2[0].x + vertical2[1].x)/2;
   //pos2.y = (vertical2[0].y + vertical2[1].y)/2;
   s1 << int(size1);
  // s2 << int(size2);
   tobe1 = s1.str();
   //tobe2 = s2.str();            
//   putText(src, tobe1, pos1,  FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,255, 0), 1,8, false );
 //  putText(src, tobe2, pos2,  FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,255, 0), 1,8, false );
 //  res = angle_cos(vertices[0], vertices[3] );
 //  stringstream s;
 //  s << int(res);
 //  tobe = s.str();      
 //  putText(src, tobe, boxes[a1].center,  FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,255, 0), 1,8, false );
 //  s.str("");
   s1.str("");
  // s2.str("");

   vector<double > answers;
   getDistances(centerOfbox, size1, size2, answers, valve, false);
   
   drawInfo(boxes[a1], answers, src);

   crds.push_back(answers);   
  }
  
  Point2f center = Point(320, 240);
  circle(src, center, 5 , Scalar(255, 0, 0), 1, 8, 0);
  circle(src, center, 1 , Scalar(255, 0, 0), -1, 8, 0);
//cout<<"src size in finddistance"<< src.size()<<endl;
  imgRes = src;
//cout<<"imgcnt size in finddistance"<< imgcnt.size()<<endl;
}


