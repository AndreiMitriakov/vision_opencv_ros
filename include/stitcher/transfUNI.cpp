
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

#include "transfUNI.h"



using namespace cv;
using namespace std;
#define PI 3.14159265
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
