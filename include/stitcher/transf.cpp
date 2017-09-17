
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

#define PI 3.14159265

using namespace cv;
using namespace std;

double find_angle(vector< vector< Point > > squares, int nmb);

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

void rotate(Mat& input, Mat& output, vector<vector<Point> >& squares, int nmb){
    //image rotation

    double angle = find_angle(squares, nmb);
    Point2f rot; rot.x = input.cols; rot.y = input.rows;
    Mat matRotation = getRotationMatrix2D( rot, angle, 1);
    Mat imgRotated;
    warpAffine( input, imgRotated, matRotation, input.size() );
}

void perspective_transformation(Mat& input, Mat& output, vector<vector<Point> >& squares, int nmb, Mat& test){

    // Realisation of perspective transformation is below
    double width, heigth;

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

    cout<<"X "<< mu[nmb].m01<<endl;
    cout<<"Y "<< mu[nmb].m10<<endl;
*/
    if(squares[nmb][0].x < squares[nmb][2].x){
	  inputQuad[0] = squares[nmb][0];//0
  	  inputQuad[1] = squares[nmb][3];//3
	  inputQuad[2] = squares[nmb][2];//2
	  inputQuad[3] = squares[nmb][1];//1

	  width = norm(Mat(squares[nmb][3]), Mat(squares[nmb][0]));
	  heigth = norm(Mat(squares[nmb][0]), Mat(squares[nmb][1]));
	//  width = abs(squares[nmb][3].x - squares[nmb][0].x);
	//  heigth =  abs(squares[nmb][0].y - squares[nmb][1].y); 

	  outputQuad[0] = Point2f( 0, 0 );
          outputQuad[1] = Point2f( width, 0 );
	  outputQuad[2] = Point2f( width, heigth );
          outputQuad[3] = Point2f( 0, heigth );
	   
    }
    else{
	  inputQuad[0] = squares[nmb][1];//1
  	  inputQuad[1] = squares[nmb][0];//0
	  inputQuad[2] = squares[nmb][3];//3
	  inputQuad[3] = squares[nmb][2];//2

	  width = norm(Mat(squares[nmb][1]), Mat(squares[nmb][0])) ;
	  heigth =  norm(Mat(squares[nmb][0]), Mat(squares[nmb][3])) ;
	  //width = abs(squares[nmb][1].x - squares[nmb][0].x);
	  //heigth =  abs(squares[nmb][0].y - squares[nmb][3].y); 

	  outputQuad[0] = Point2f( 0, 0 );
          outputQuad[1] = Point2f( width, 0 );
	  outputQuad[2] = Point2f( width, heigth );
          outputQuad[3] = Point2f( 0, heigth );
    }
//cout<<"First "<< squares[nmb][0]<<" Second "<<squares[nmb][1]<<" Third "<<squares[nmb][2]<<" Fourth "<<squares[nmb][3]<<endl;
    // The 4 points where the mapping is to be done , from top-left in clockwise order

    // Get the Perspective Transform Matrix i.e. lambda 
    lambda = getPerspectiveTransform( inputQuad, outputQuad);
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,input.size());
   
    //If the picture is vertical, we make her horizontal

    Point min, max;

    min.x = 0; min.y = 0;
    max.x = width; max.y = heigth;

cout<<"IN"<<endl;
    output = output(Rect(min, max));
cout<<"OUT"<<endl;  
    //checking of orientation using vertical part of digits



    //If the picture is vertical, we make her horizontal
    
}

double find_angle(vector< vector< Point > > squares, int nmb){

	Point ptsLow[2];
	//the lowest points finding
	ptsLow[0] = squares[nmb][0];
	ptsLow[1] = squares[nmb][1];	
	
	for(int i = 1; i < 4; i++)
		if((ptsLow[1].y > squares[nmb][i].y) && (squares[nmb][i].y > ptsLow[0].y))
			ptsLow[1] = squares[nmb][i];
	

	//finding of angle
	double angle = atan ( double (ptsLow[0].y-ptsLow[1].y)/ double (ptsLow[0].x-ptsLow[1].x) )/ PI * 180;

	
	return angle;
}
