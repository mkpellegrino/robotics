#include <stdio.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

/*
 * main.cpp
 *
 *  Created on: Mar 11, 2022
 *      Author: mkpellegrino
 *      Uses OpenCV to recognize a piece of
 *      Retro-reflective tape that is 173mm x 50mm
 *      and how far away it is from the webcam
 */
int main(int, char**)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//vector<Rect> possibleTargets;

	cv::VideoCapture camera(0);
	if (!camera.isOpened())
	{
	   std::cerr << "ERROR: Could not open camera" << std::endl;
	   return 1;
	}
    cv::namedWindow("Webcam", 0);
    cv::Mat frame;
    cv::Mat img1;
    //cv::Mat img2;
    while (1)
    {
    	camera >> img1;
        blur( img1, frame, Size( 5, 5 ) );
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        threshold(frame, frame, 150, 255, THRESH_BINARY);
        findContours(frame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        //drawContours(img1, contours, -1, Scalar(0, 255, 0), 2);
        for( int i=0; i<contours.size(); i++)
        		{
        			Rect r = boundingRect(contours[i]);
        			if( abs( (r.height/r.width) - 3.46 ) < 1 ) // the test strip has the ratio: h/w==3.46
        			{
        				rectangle(img1, Point(r.x, r.y), Point( r.x+r.width, r.y+r.height), Scalar( 0,0,255));
        				// when height is 500, distance is 12 inches
        				// when height is 100, distance is 64 inches
        				putText(img1, to_string( -0.13*((double)r.height)+77), Point( r.x, r.y ), FONT_HERSHEY_DUPLEX, 1.0, Scalar( 255, 255, 255 ));
        				//possibleTargets.push_back(r);
        			}
        			//else
        			//{
        				//rectangle(img1, Point(r.x, r.y), Point( r.x+r.width, r.y+r.height), Scalar( 0,255,0));
        			//}
        		}
        //putText(img1, "FRC 5752", Point( 1024, 640 ), FONT_HERSHEY_DUPLEX, 1.0, Scalar( 255, 0, 000 ));

    	cv::imshow("Webcam", img1);
    	// wait (10ms) for a key to be pressed
    	if (cv::waitKey(10) >= 0)
    		break;
    }

    return 0;
}



