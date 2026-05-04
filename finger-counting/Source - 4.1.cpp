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

//////////////////// DEFINE ////////////////////

#define ID_CAM 2
#define INIT_TIME 60
#define ZETA 10
#define radius 1.65

//////////////////// DECLARE ////////////////////

CvCapture* capture = 0;

CvSize size_full = cvSize(640, 480);
CvSize size_small = cvSize(320, 240);

IplImage* lower = cvCreateImage(size_small, IPL_DEPTH_32F, 3);
IplImage* upper = cvCreateImage(size_small, IPL_DEPTH_32F, 3);

IplImage* frame = 0;
IplImage* frame_small = cvCreateImage(size_small, 8, 3);
IplImage* hand = cvCreateImage(size_small, 8, 3);

IplImage* msk = cvCreateImage(size_small, 8, 1);
IplImage* msk_fg = cvCreateImage(size_small, 8, 1);
IplImage* msk_color = cvCreateImage(size_small, 8, 1);
IplImage* msk_bgsubtract = cvCreateImage(size_small, 8, 1);

IplImage* mask = cvCreateImage(size_full, 8, 3);
IplImage* draw_mask = cvCreateImage(size_full, 8, 3);
IplImage* symbol_mask = cvCreateImage(size_full, 8, 1);
IplImage* symbol = cvCreateImage(size_small, 8, 1);

IplImage* flv_frame = cvCreateImage(cvSize(1280, 480), 8, 3);

//////////////////// BLOB ////////////////////
// pt is the center
// pt1 is the top left of blob
// pt2 is the farthest point

CvPoint pt, pt1, pt2, last_pt2 = cvPoint(0, 0);

// max is max distant tranform value
float max_DT;

//////////////////// TIME MEASURE ////////////////////

int64 now, then;
//double elapsed_time, ticks = cvGetTickFrequency() * 1.0e6;
double ticks = cvGetTickFrequency() * 1.0e6;
double frame_time, hand_time, finger_time, track_time;

//////////////////// INIT FONT ////////////////////

CvFont font;
CvScalar color = CV_RGB(255, 0, 0);

//////////////////// CONTROL INTERFACE ////////////////////

char last_num_fingers = 0, num_fingers = 0, rs_mask = 0;

//////////////////// FUNCTION ////////////////////

void guide();
void bg_init(IplImage* dst1, IplImage* dst2);
void bg_subtraction(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst);
void bg(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst);

/////////////////////////////////////////////////////////////////////////////////////////
////////                              MAIN PROGRAM                               ////////
/////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	// hight priority class
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	// initial mode
	int hand_mode = 0;
	// initial windows
	const char* camera_name = "Tracking";
	//const char* hand_name = "Hand";

	const char* bg_name = "Background Subtraction";
	const char* skin_name = "Skin Color";
	//const char* fg_name = "Foreground";

	cvNamedWindow(camera_name, CV_WINDOW_AUTOSIZE);
	//cvNamedWindow(hand_name, CV_WINDOW_AUTOSIZE);

	cvNamedWindow(bg_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(skin_name, CV_WINDOW_AUTOSIZE);
	//cvNamedWindow(fg_name, CV_WINDOW_AUTOSIZE);

	// connect camera
	capture = cvCaptureFromCAM(ID_CAM);
	if (!capture)  printf("Could not initialize capturing...\n");

	// show guide
	guide();

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////// MAIN PROCESSING LOOP ///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////// 

	while (1)
	{
		then = cvGetTickCount();

		if (hand_mode == 1)
		{
			bg_init(lower, upper);
			hand_mode = 2;
			rs_mask = 0;
		}

		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);

		cvFlip(frame, frame, 1);
		cvResize(frame, frame_small, 1);

		if (hand_mode == 2)
		{
			// BACKGROUND SUBTRACTION 
			bg_subtraction(frame_small, lower, upper, msk_bgsubtract);
			bg(frame_small, lower, upper, msk_color);
			
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// DISPLAY ////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////// 

		cvShowImage(camera_name, frame_small);
		//cvShowImage(hand_name, hand);

		cvShowImage(bg_name, msk_bgsubtract);
		cvShowImage(skin_name, msk_color);
		//cvShowImage(fg_name, msk_fg);

		/////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////// USER INPUT ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////

		char c = cvWaitKey(50);
		if ((char)c == 9)
		{
			hand_mode = 1;
		}
		else if ((char)c == 27)	break;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// RELEASE CAPTURE & IMAGE //////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	cvReleaseCapture(&capture);
	cvDestroyWindow(camera_name);
	//cvDestroyWindow(hand_name);

	//cvDestroyWindow(fg_name);
	cvDestroyWindow(bg_name);
	cvDestroyWindow(skin_name);
}

//////////////////// GUIDE ////////////////////

void guide()
{
	printf("=========================================================\n"
		"================ DAI HOC BACH KHOA TPHCM ================\n"
		"================== KHOA DIEN - DIEN TU ==================\n"
		"===================== NGANH TU DONG =====================\n"
		"=========================================================\n"
		"=================== LUAN VAN CAO HOC ====================\n"
		"=========================================================\n"
		"==================== HVTH: TRAN HOAN ====================\n"
		"===================== MSHV: 7140954 =====================\n"
		"=========================================================\n"
		);
	printf("GUIDE: \n"
		"\tESC - Quit the program \n"
		"\tTab - Go to Hand Gesture Recognition \n");
}

//////////////////// BACKGROUND INITIAL ////////////////////

void bg_init(IplImage* dst1, IplImage* dst2)
{
	IplImage* tmp_1 = cvCreateImage(size_small, IPL_DEPTH_32F, 3);
	IplImage* tmp_2 = cvCreateImage(size_small, IPL_DEPTH_32F, 3);
	IplImage* tmp_3 = cvCreateImage(size_small, 8, 3);
	IplImage* tmp_4 = cvCreateImage(size_small, IPL_DEPTH_32F, 3);

	cvSetZero(tmp_1);
	for (int i = 0; i < INIT_TIME; i++)
	{
		frame = cvQueryFrame(capture);
		if (frame == 0) i--;
		cvFlip(frame, frame, 1);
		cvResize(frame, tmp_3);
		cvAcc(tmp_3, tmp_1);
	}
	cvConvertScale(tmp_1, tmp_1, 1.0 / INIT_TIME);

	cvSetZero(tmp_2);
	for (int j = 0; j < INIT_TIME; j++)
	{
		frame = cvQueryFrame(capture);
		if (frame == 0) j--;
		cvFlip(frame, frame, 1);
		cvResize(frame, tmp_3);
		cvConvertScale(tmp_3, tmp_4);
		cvSub(tmp_4, tmp_1, tmp_4);
		cvPow(tmp_4, tmp_4, 2.0);
		cvPow(tmp_4, tmp_4, 0.5);
		cvAcc(tmp_4, tmp_2);
	}
	cvConvertScale(tmp_2, tmp_2, 1.0 / INIT_TIME);

	cvSub(tmp_1, tmp_2, dst1);
	cvSubS(dst1, cvScalarAll(ZETA), dst1);

	cvAdd(tmp_1, tmp_2, dst2);
	cvAddS(dst2, cvScalarAll(ZETA), dst2);
}

//////////////////// BACKGROUND SUBTRACTION ////////////////////

void bg_subtraction(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst)
{
	IplImage* tmp = cvCreateImage(size_small, IPL_DEPTH_32F, 3);
	cvConvertScale(src1, tmp);
	// display different
	cvInRange(tmp, src2, src3, dst);
	cvNot(dst, dst);
	cvErode(dst, dst, 0, 2);
	cvDilate(dst, dst, 0, 2);
}

void bg(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst)
{
	IplImage* tmp = cvCreateImage(size_small, IPL_DEPTH_32F, 3);
	cvConvertScale(src1, tmp);
	// display different
	cvInRange(tmp, src2, src3, dst);
	cvNot(dst, dst);
	//cvErode(dst, dst, 0, 2);
	//cvDilate(dst, dst, 0, 2);
}