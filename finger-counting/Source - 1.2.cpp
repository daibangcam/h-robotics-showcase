#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <windows.h>
#include <highgui.h>
#include <stdlib.h>   
#include <cxcore.h>
#include <cxmisc.h> 
#include <cvaux.h>
#include <ctype.h> 
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <ml.h>

using namespace cv;
using namespace std;

#define h 240
#define w 320

int main()
{
	// log
	fstream parameter;
	parameter.open("_parameters.csv", ios::app);
	// Image variables
	IplImage* img = cvLoadImage("1.jpg");
	IplImage* im = cvCreateImage(cvSize(w, h), 8, 3);
	IplImage* hsv = cvCreateImage(cvSize(w, h), 8, 3);
	IplImage* yuv = cvCreateImage(cvSize(w, h), 8, 3);
	IplImage* hsv_thresh = cvCreateImage(cvSize(w, h), 8, 1);
	IplImage* yuv_thresh = cvCreateImage(cvSize(w, h), 8, 1);
	// Windows
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("HSV", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("HSV Trackbar", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("YUV", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("YUV Trackbar", CV_WINDOW_AUTOSIZE);
	// Variables for HSV trackbar
	int h1 = 0, h2 = 20, s1 = 45, s2 = 255, v_1 = 0, v_2 = 135;
	//Variables for YUV trackbar
	int y1 = 0, y2 = 255, u1 = 131, u2 = 185, v1 = 80, v2 = 135;
	// Creating the HSV trackbars
	cvCreateTrackbar("H1", "HSV Trackbar", &h1, 255, 0);
	cvCreateTrackbar("H2", "HSV Trackbar", &h2, 255, 0);
	cvCreateTrackbar("S1", "HSV Trackbar", &s1, 255, 0);
	cvCreateTrackbar("S2", "HSV Trackbar", &s2, 255, 0);
	cvCreateTrackbar("V1", "HSV Trackbar", &v_1, 255, 0);
	cvCreateTrackbar("V2", "HSV Trackbar", &v_2, 255, 0);
	// Creating the YUV trackbars
	cvCreateTrackbar("Y1", "YUV Trackbar", &y1, 255, 0);
	cvCreateTrackbar("Y2", "YUV Trackbar", &y2, 255, 0);
	cvCreateTrackbar("U1", "YUV Trackbar", &u1, 255, 0);
	cvCreateTrackbar("U2", "YUV Trackbar", &u2, 255, 0);
	cvCreateTrackbar("V1", "YUV Trackbar", &v1, 255, 0);
	cvCreateTrackbar("V2", "YUV Trackbar", &v2, 255, 0);
	// Resizing the image
	cvResize(img, im, 1);
	cvSmooth(im, im, CV_GAUSSIAN, 5);
	// Changing into HSV plane	
	cvCvtColor(im, hsv, CV_BGR2HSV);
	// Changing into YUV plane
	cvCvtColor(im, yuv, CV_BGR2YCrCb);
	while (1)
	{
		// Thresholding the image
		cvInRangeS(hsv, cvScalar(h1, s1, v_1), cvScalar(h2, s2, v_2), hsv_thresh);
		cvDilate(hsv_thresh, hsv_thresh, 0, 2);
		cvErode(hsv_thresh, hsv_thresh, 0, 3);
		cvInRangeS(yuv, cvScalar(y1, u1, v1), cvScalar(y2, u2, v2), yuv_thresh);
		cvDilate(yuv_thresh, yuv_thresh, 0, 2);
		cvErode(yuv_thresh, yuv_thresh, 0, 3);
		// Showing the images
		cvShowImage("Original", im);
		cvShowImage("HSV", hsv_thresh);
		cvShowImage("YUV", yuv_thresh);
		// Escape Sequence
		char c = cvWaitKey(33);
		if (c == 27)
			break;
	}
	// Showing the image
	cvShowImage("Original", im);
	cvShowImage("HSV", hsv_thresh);
	cvShowImage("YUV", yuv_thresh);
	// log
	parameter << y1 << "," << y2 << "," << u1 << "," << u2 << "," << v1 << "," << v2 << ","
		<< "," << h1 << "," << h2 << "," << s1 << "," << s2 << "," << v_1 << "," << v_2 << endl;
	// Waiting for user to press any key
	cvWaitKey(0);
	// Cleanup
	cvReleaseImage(&img);
	cvReleaseImage(&im);
	cvReleaseImage(&hsv);
	cvReleaseImage(&hsv_thresh);
	cvReleaseImage(&yuv);
	cvReleaseImage(&yuv_thresh);
	cvDestroyAllWindows();
}