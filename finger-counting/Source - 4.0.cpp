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

#include "BlobLabeling.h"
#include "BlobLabeling.cpp"

using namespace cv;
using namespace std;

CvSize size_small = cvSize(320, 240);

void skin_yuv_color(IplImage* src, IplImage* dst, int y1, int y2, int u1, int u2, int v1, int v2);
void skin_hsv_color(IplImage* src, IplImage* dst, int h1, int h2, int s1, int s2, int v1, int v2);
void skin_yuv(IplImage* src, IplImage* dst, int y1, int y2, int u1, int u2, int v1, int v2);

void main()
{

	IplImage* frame = 0;
	IplImage* frame_small = cvCreateImage(size_small, 8, 3);
	IplImage* yuv_mask = cvCreateImage(size_small, 8, 1);
	//IplImage* hsv_mask = cvCreateImage(size_small, 8, 1);
	IplImage* yuv = cvCreateImage(size_small, 8, 1);

	//CvCapture* capture = cvCaptureFromFile("input.mp4");
	CvCapture* capture = cvCaptureFromCAM(2);

	int y1 = 0, y2 = 255, u1 = 131, u2 = 185, v1 = 80, v2 = 135;
	cvNamedWindow("YUV Trackbar", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Y1", "YUV Trackbar", &y1, 255, 0);
	cvCreateTrackbar("Y2", "YUV Trackbar", &y2, 255, 0);
	cvCreateTrackbar("U1", "YUV Trackbar", &u1, 255, 0);
	cvCreateTrackbar("U2", "YUV Trackbar", &u2, 255, 0);
	cvCreateTrackbar("V1", "YUV Trackbar", &v1, 255, 0);
	cvCreateTrackbar("V2", "YUV Trackbar", &v2, 255, 0);

	//int h1 = 0, h2 = 20, s1 = 45, s2 = 255, v_1 = 0, v_2 = 135;
	//cvNamedWindow("HSV Trackbar", CV_WINDOW_AUTOSIZE);
	//cvCreateTrackbar("Y1", "HSV Trackbar", &h1, 255, 0);
	//cvCreateTrackbar("Y2", "HSV Trackbar", &h2, 255, 0);
	//cvCreateTrackbar("U1", "HSV Trackbar", &s1, 255, 0);
	//cvCreateTrackbar("U2", "HSV Trackbar", &s2, 255, 0);
	//cvCreateTrackbar("V1", "HSV Trackbar", &v_1, 255, 0);
	//cvCreateTrackbar("V2", "HSV Trackbar", &v_2, 255, 0);

	//cvNamedWindow("Tracking", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("HSV", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("YUV", CV_WINDOW_AUTOSIZE);

	// log
	//fstream parameter;
	//parameter.open("_parameters.csv", ios::app);
	//parameter << "y1, y2, u1, u2, v1, v2, , h1, h2, s1, s2, v1, v2" << endl;

	while (1)
	{
		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);

		cvFlip(frame, frame, 1);
		cvResize(frame, frame_small, 1);

		skin_yuv_color(frame_small, yuv_mask, y1, y2, u1, u2, v1, v2);
		//skin_hsv_color(frame_small, hsv_mask, h1, h2, s1, s2, v_1, v_2);
		skin_yuv(frame_small, yuv, y1, y2, u1, u2, v1, v2);

		cvShowImage("Tracking", frame_small);
		cvShowImage("YUV", yuv_mask);
		//cvShowImage("HSV", hsv_mask);
		cvShowImage("YUV Original", yuv);

		//Escape Sequence
		char c = cvWaitKey(1);
		if (c == 27)
			break;
	}

	//parameter << y1 << "," << y2 << "," << u1 << "," << u2 << "," << v1 << "," << v2 << ","
	//	<< "," << h1 << "," << h2 << "," << s1 << "," << s2 << "," << v_1 << "," << v_2 << endl;

	//parameter.close();

	//Cleanup	
	cvReleaseCapture(&capture);
	cvDestroyWindow("Tracking");
	cvDestroyAllWindows();
}

void skin_yuv_color(IplImage* src, IplImage* dst, int y1, int y2, int u1, int u2, int v1, int v2)
{
	IplImage* yuv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, yuv, CV_GAUSSIAN, 5);
	cvCvtColor(yuv, yuv, CV_BGR2YCrCb);
	cvInRangeS(yuv, cvScalar(y1, u1, v1), cvScalar(y2, u2, v2), dst);
	cvErode(dst, dst, 0, 3);
	cvDilate(dst, dst, 0, 3);
	//cvErode(dst, dst, 0, 3);
}

void skin_yuv(IplImage* src, IplImage* dst, int y1, int y2, int u1, int u2, int v1, int v2)
{
	IplImage* yuv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, yuv, CV_GAUSSIAN, 5);
	cvCvtColor(yuv, yuv, CV_BGR2YCrCb);
	cvInRangeS(yuv, cvScalar(y1, u1, v1), cvScalar(y2, u2, v2), dst);
	//cvDilate(dst, dst, 0, 2);
	//cvErode(dst, dst, 0, 3);
}

void skin_hsv_color(IplImage* src, IplImage* dst, int h1, int h2, int s1, int s2, int v1, int v2)
{
	IplImage* hsv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, hsv, CV_GAUSSIAN, 5);
	cvCvtColor(hsv, hsv, CV_BGR2HSV);
	cvInRangeS(hsv, cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), dst);
	cvErode(dst, dst, 0, 2);
	cvDilate(dst, dst, 0, 3);
}