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

//#define SMA_FLAG 0
//#define EMA_FLAG 1

#define w 640
#define h 480
#define w_s 320
#define h_s 240

#define init_time 20
#define zeta 10

#define PI 3.14159265
#define radius 1.65

#define N 5
#define num 1000

#define no_fingers 45
#define threshold_fingers 20
#define threshold_blob_height 120
#define threshold_blob_width 160

#define ID_CAM 1

//////////////////// BLOB ////////////////////

// pt is the center
// pt1 is the top left of blob
// pt2 is the farthest point

CvPoint pt, pt1, pt2;

//////////////////// CONTROL INTERFACE ////////////////////

unsigned char num_fingers;

//////////////////// EMA ////////////////////

unsigned short int LastX0, LastX1, LastX2, LastX3, LastX4;
unsigned short int LastY0, LastY1, LastY2, LastY3, LastY4;

//////////////////// SMA ////////////////////

//unsigned short int LastX5, LastX6, LastX7, LastX8, LastX9;
//unsigned short int LastY5, LastY6, LastY7, LastY8, LastY9;

//////////////////// FUNCTION ////////////////////

void guide();
void bg_init(CvCapture* capture, IplImage* dst1, IplImage* dst2);
void bg_subtraction(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst);
void skin_yuv_color(IplImage* src, IplImage* dst);
//void skin_hsv_color(IplImage* src, IplImage* dst);
void detect_hand(IplImage* src, IplImage* lower, IplImage* upper, IplImage* mask, IplImage* msk_bgsubtract, IplImage* msk_color, IplImage* msk_fg, IplImage* dst);
void detect_fingers(IplImage* src, IplImage* dst);
void reset_mask(IplImage* msk1, IplImage* msk2);
//void update_SMA(int x, int y);
void update_EMA(int x, int y);
void EMA(unsigned char EMA_Init, float E_SMA_X, float E_SMA_Y, float EMA_X, float EMA_Y);

/////////////////////////////////////////////////////////////////////////////////////////
////////                              MAIN PROGRAM                               ////////
/////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	// Hight Priority Class
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	// Control Interface
	unsigned char rs_mask;
	unsigned char last_num_fingers = 0;
	CvPoint last_pt2 = cvPoint(0, 0);

	// Initial Mode
	unsigned char hand_mode = 1;

	// Initial Color
	CvScalar color_red = CV_RGB(255, 0, 0);
	CvScalar color_white = CV_RGB(255, 255, 255);

	// Initial Coordinates
	//unsigned short int coordinates[num];
	//unsigned short int num_coordinates = 0;
	//
	//signed short int delta_x, delta_y, angle;
	double distance;
	//
	//signed short int draw_coordinates[num/2];
	//unsigned short int num_draw_coordinates = 0;	

	// Time Measure
	//int64 now, then;
	//float ticks = cvGetTickFrequency() * 1.0e3;
	//float frame_time, hand_time, finger_time, track_time;
	//frame_time = hand_time = finger_time = track_time = 0;

	// EMA Variable
	float E_SMA_X, E_SMA_Y, EMA_X, EMA_Y;
	unsigned char EMA_Init = 0;
	EMA_X = EMA_Y = 0.0;

	// SMA Variable
	//	float SMA_X, SMA_Y;
	//	unsigned char SMA_Init = 0;
	//	SMA_X = SMA_Y = 0.0;

	//// Log Time
	//ofstream process;
	//process.open("_log_processing_time.csv");
	//process << "Frame, Hand, Finger, Track, FPS" << endl;

	//// Log Coordinates
	//ofstream coordinate;
	//coordinate.open("_log_coordinates.csv");
	//coordinate << "x, y" << endl;

	//// Log Draw Coordinates
	//ofstream draw_coordinate;
	//draw_coordinate.open("_log_draw_coordinates.csv");
	//draw_coordinate << "angle" << endl;

	// Log EMA Coordinates
	//ofstream EMA_coordinate;
	//EMA_coordinate.open("_log_EMA_coordinates.csv");
	//EMA_coordinate << "x, y" << endl;

	// Log SMA Coordinates
	//ofstream SMA_coordinate;
	//SMA_coordinate.open("_log_SMA_coordinates.csv");
	//SMA_coordinate << "x, y" << endl;

	// Input Video Writer
	//CvVideoWriter *writer_input = cvCreateVideoWriter("_video_input.avi", CV_FOURCC('M', 'J', 'P', 'G'), 13, cvSize(640, 480));

	// Output Video Writer
	//CvVideoWriter *writer_output = cvCreateVideoWriter("_video_output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, cvSize(640, 480));

	// Declare Images
	CvCapture* capture = 0;
	IplImage* lower = cvCreateImage(cvSize(w_s, h_s), IPL_DEPTH_32F, 3);
	IplImage* upper = cvCreateImage(cvSize(w_s, h_s), IPL_DEPTH_32F, 3);
	IplImage* frame;
	IplImage* frame_small = cvCreateImage(cvSize(w_s, h_s), 8, 3);
	IplImage* hand = cvCreateImage(cvSize(w_s, h_s), 8, 3);
	IplImage* msk = cvCreateImage(cvSize(w_s, h_s), 8, 1);
	IplImage* msk_fg = cvCreateImage(cvSize(w_s, h_s), 8, 1);
	IplImage* msk_color = cvCreateImage(cvSize(w_s, h_s), 8, 1);
	IplImage* msk_bgsubtract = cvCreateImage(cvSize(w_s, h_s), 8, 1);
	IplImage* mask = cvCreateImage(cvSize(w, h), 8, 3);
	IplImage* draw_mask = cvCreateImage(cvSize(w, h), 8, 3);
	//IplImage* symbol_mask = cvCreateImage(cvSize(w, h), 8, 1);
	//IplImage* symbol = cvCreateImage(cvSize(w_s, h_s), 8, 1);

	// Initial Windows
	const char* camera_name = "Tracking";
	const char* hand_name = "Hand";
	cvNamedWindow(camera_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(hand_name, CV_WINDOW_AUTOSIZE);

	const char* bg_name = "Background Subtraction";
	const char* skin_name = "Skin Color";
	const char* fg_name = "Foreground";
	cvNamedWindow(bg_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(skin_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(fg_name, CV_WINDOW_AUTOSIZE);

	// Load Video
	capture = cvCaptureFromFile("input_4.mp4");
	//if (!capture)
	//{
	//	printf("Error!!! Could not load video...\n");
	//	getchar();
	//}

	// Connect Camera
	//capture = cvCaptureFromCAM(ID_CAM);
	//if (!capture)
	//{
	//	printf("Error!!! Could not initialize capturing...\n");
	//	getchar();
	//}

	// Show Guide
	guide();

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////// MAIN PROCESSING LOOP ///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////// 

	while (1)
	{
		// tick time
		//then = cvGetTickCount();

		// init setup
		if (hand_mode == 1)
		{
			bg_init(capture, lower, upper);
			hand_mode = 2;
			rs_mask = 0;

			LastX0 = LastX1 = LastX2 = LastX3 = LastX4 = 0;
			LastY0 = LastY1 = LastY2 = LastY3 = LastY4 = 0;

			//LastX5 = LastX6 = LastX7 = LastX8 = LastX9 = 0;
			//LastY5 = LastY6 = LastY7 = LastY8 = LastY9 = 0;
		}

		// get frame
		//do
		//{
		frame = cvQueryFrame(capture);
		//} while (!frame);
		if (!frame) break;

		//// get frame time
		//now = cvGetTickCount();
		//frame_time = (now - then) / ticks;

		// save to video input
		//cvWriteFrame(writer_input, frame);

		// flip and resize frame
		//cvFlip(frame, frame, 1);
		cvResize(frame, frame_small, 1);

		// draw mode
		if (hand_mode == 2)
		{
			// detect hand
			detect_hand(frame_small, lower, upper, msk, msk_bgsubtract, msk_color, msk_fg, hand);
			// get hand time
			//now = cvGetTickCount();
			//hand_time = (now - then) / ticks;

			// detect fingers
			detect_fingers(msk, frame);
			// get finger time
			//now = cvGetTickCount();
			//finger_time = (now - then) / ticks;

			// convert x,y			
			pt2.x = pt2.x * 2;
			pt2.y = pt2.y * 2;

			/////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////  EXPONENTIAL MOVING AVERAGE CALCULATION   ///////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////

			//#if (SMA_FLAG)
			//			if (SMA_Init < N)
			//			{
			//				update_SMA(pt2.x, pt2.y);
			//				SMA_Init++;
			//				SMA_X = (LastX5 + LastX6 + LastX7 + LastX8 + LastX9) / SMA_Init;
			//				SMA_Y = (LastY5 + LastY6 + LastY7 + LastY8 + LastY9) / SMA_Init;
			//			}
			//			else
			//			{
			//				update_SMA(pt2.x, pt2.y);
			//				SMA_X = (LastX5 + LastX6 + LastX7 + LastX8 + LastX9) / N;
			//				SMA_Y = (LastY5 + LastY6 + LastY7 + LastY8 + LastY9) / N;
			//			}
			//
			//			// log SMA coordinates			
			//			SMA_coordinate << SMA_X << "," << SMA_Y << endl;
			//#else
			//#endif

			//#if (EMA_FLAG)
			if (EMA_Init <= N)
			{
				update_EMA(pt2.x, pt2.y);
				EMA_Init++;
				if (EMA_Init == N)
				{
					E_SMA_X = (LastX0 + LastX1 + LastX2 + LastX3 + LastX4) / N;
					E_SMA_Y = (LastY0 + LastY1 + LastY2 + LastY3 + LastY4) / N;
					EMA_X = E_SMA_X;
					EMA_Y = E_SMA_Y;
				}
			}
			else
			{
				EMA_X = (((float)pt2.x - EMA_X) * 2 / (N + 1)) + EMA_X;
				EMA_Y = (((float)pt2.y - EMA_Y) * 2 / (N + 1)) + EMA_Y;
				pt2.x = EMA_X;
				pt2.y = EMA_Y;
			}
			//log EMA coordinates			
			//EMA_coordinate << EMA_X << "," << EMA_Y << endl;
			//#else
			//#endif

			/////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////// DRAW //////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////

			// reset mask
			if (rs_mask == 0)
			{
				reset_mask(mask, draw_mask);
				rs_mask = 1;
			}

			// limit distance between 2 frame for drawing
			distance = sqrt((pt2.x - last_pt2.x)*(pt2.x - last_pt2.x) + (pt2.y - last_pt2.y)*(pt2.y - last_pt2.y));

			// drawing
			if (num_fingers == 1 && distance > 0 && distance < 60)
			{
				//// log coordinates	
				//coordinate << pt2.x << "," << h - pt2.y << endl;

				// draw on frame
				cvLine(mask, last_pt2, pt2, color_red, 10);						// draw line	
				cvLine(draw_mask, last_pt2, pt2, color_white, 10);				// for mask
				//cvLine(symbol_mask, last_pt2, pt2, color_white, 10);			// for hand_gestures recognition											

				//// save coordinates to array
				//coordinates[num_coordinates] = pt2.x;
				//coordinates[num_coordinates + 1] = h - pt2.y;
				//num_coordinates += 2;
			}

			//	processing
			if (last_num_fingers == 2 && num_fingers == 3)
			{
				//cvResize(symbol_mask, symbol);
				//cvShowImage("Symbol", symbol);
				//for (int i = 0; i < num_coordinates - 3; i += 2)
				//{
				//	// calculate angles
				//	delta_x = coordinates[i + 2] - coordinates[i];
				//	delta_y = coordinates[i + 3] - coordinates[i + 1];
				//	angle = round(atan2(delta_x, delta_y) * 180 / PI);

				//	// log angles
				//	draw_coordinate << angle << endl;

				//	// save angles to array
				//	draw_coordinates[num_draw_coordinates] = angle;
				//	num_draw_coordinates++;
				//}
				//num_coordinates = 0;
			}

			// clear all when the number of finger == 5
			if (num_fingers == 5)
			{
				reset_mask(mask, draw_mask);
			}

			// draw line for finger writing 
			cvLine(mask, cvPoint(300, 120), cvPoint(620, 120), color_white, 2);
			cvLine(draw_mask, cvPoint(300, 120), cvPoint(620, 120), color_white, 1);
			cvLine(mask, cvPoint(300, 170), cvPoint(620, 170), color_white, 2);
			cvLine(draw_mask, cvPoint(300, 170), cvPoint(620, 170), color_white, 1);
			cvLine(mask, cvPoint(300, 220), cvPoint(620, 220), color_white, 2);
			cvLine(draw_mask, cvPoint(300, 220), cvPoint(620, 220), color_white, 1);
			cvLine(mask, cvPoint(300, 270), cvPoint(620, 270), color_white, 2);
			cvLine(draw_mask, cvPoint(300, 270), cvPoint(620, 270), color_white, 1);
			cvLine(mask, cvPoint(300, 320), cvPoint(620, 320), color_white, 2);
			cvLine(draw_mask, cvPoint(300, 320), cvPoint(620, 320), color_white, 1);

			// show draw on frame
			cvCopy(mask, frame, draw_mask);

			// update coordinate
			last_pt2 = pt2;
			last_num_fingers = num_fingers;

			// save to video
			//cvWriteFrame(writer_output, frame);
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// DISPLAY ////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////// 

		cvShowImage(camera_name, frame);
		cvShowImage(hand_name, hand);

		cvShowImage(bg_name, msk_bgsubtract);
		cvShowImage(skin_name, msk_color);
		cvShowImage(fg_name, msk_fg);

		/////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////// USER INPUT ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////

		// get track time
		//now = cvGetTickCount();
		//track_time = (now - then) / ticks;
		//process << frame_time << "," << hand_time << "," << finger_time << "," << track_time << "," << 1000 / track_time << endl;

		// wait key
		char c = cvWaitKey(1);
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
	cvDestroyWindow(hand_name);

	//cvDestroyWindow(fg_name);
	//cvDestroyWindow(bg_name);
	//cvDestroyWindow(skin_name);

	//cvReleaseVideoWriter(&writer_input);
	//cvReleaseVideoWriter(&writer_output);

	//EMA_coordinate.close();
	//SMA_coordinate.close();
	//process.close();
	//coordinate.close();
	//draw_coordinate.close();
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
	printf("\nGUIDE: \n"
		"\tESC - Quit the program \n"
		"\tTab - Go to Hand Gesture Recognition \n");
}

//////////////////// BACKGROUND INITIAL ////////////////////

void bg_init(CvCapture* capture, IplImage* dst1, IplImage* dst2)
{
	IplImage* tmp_1 = cvCreateImage(cvSize(w_s, h_s), IPL_DEPTH_32F, 3);
	IplImage* tmp_2 = cvCreateImage(cvSize(w_s, h_s), IPL_DEPTH_32F, 3);
	IplImage* tmp_3 = cvCreateImage(cvSize(w_s, h_s), 8, 3);
	IplImage* tmp_4 = cvCreateImage(cvSize(w_s, h_s), IPL_DEPTH_32F, 3);
	IplImage* frame;

	cvSetZero(tmp_1);
	for (int i = 0; i < init_time; i++)
	{
		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);
		if (frame == 0) i--;
		//cvFlip(frame, frame, 1);
		cvResize(frame, tmp_3);
		cvAcc(tmp_3, tmp_1);
	}
	cvConvertScale(tmp_1, tmp_1, 1.0 / init_time);

	cvSetZero(tmp_2);
	for (int j = 0; j<init_time; j++)
	{
		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);
		if (frame == 0) j--;
		//cvFlip(frame, frame, 1);
		cvResize(frame, tmp_3);
		cvConvertScale(tmp_3, tmp_4);
		cvSub(tmp_4, tmp_1, tmp_4);
		cvPow(tmp_4, tmp_4, 2.0);
		cvPow(tmp_4, tmp_4, 0.5);
		cvAcc(tmp_4, tmp_2);
	}
	cvConvertScale(tmp_2, tmp_2, 1.0 / init_time);

	cvSub(tmp_1, tmp_2, dst1);
	cvSubS(dst1, cvScalarAll(zeta), dst1);

	cvAdd(tmp_1, tmp_2, dst2);
	cvAddS(dst2, cvScalarAll(zeta), dst2);
}

//////////////////// BACKGROUND SUBTRACTION ////////////////////

void bg_subtraction(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst)
{
	IplImage* tmp = cvCreateImage(cvSize(w_s, h_s), IPL_DEPTH_32F, 3);
	cvConvertScale(src1, tmp);
	// display different
	cvInRange(tmp, src2, src3, dst);
	cvNot(dst, dst);
	cvErode(dst, dst, 0, 2);
	cvDilate(dst, dst, 0, 2);
}

//////////////////// COLOR SEGMENTATION ////////////////////

void skin_yuv_color(IplImage* src, IplImage* dst)
{
	IplImage* yuv = cvCreateImage(cvSize(w_s, h_s), 8, 3);
	cvSmooth(src, yuv, CV_GAUSSIAN, 5);
	cvCvtColor(yuv, yuv, CV_BGR2YCrCb);
	//cvInRangeS(yuv, cvScalar(50, 131, 80), cvScalar(245, 185, 135), dst);		// skin 1
	//cvInRangeS(yuv, cvScalar(30, 131, 50), cvScalar(245, 185, 120), dst);		// skin 2
	cvInRangeS(yuv, cvScalar(0, 131, 80), cvScalar(255, 185, 135), dst);		// skin 3	
	cvDilate(dst, dst, 0, 2);
	cvErode(dst, dst, 0, 3);
}

//void skin_hsv_color(IplImage* src, IplImage* dst)
//{
//	IplImage* hsv = cvCreateImage(cvSize(w_s,h_s), 8, 3);
//	cvSmooth(src, hsv, CV_GAUSSIAN, 5);
//	cvCvtColor(hsv, hsv, CV_BGR2HSV);
//	//cvInRangeS(hsv, cvScalar(0, 45, 0), cvScalar(20, 255, 255), dst);			//bright skin
//	cvInRangeS(hsv, cvScalar(0, 45, 0), cvScalar(20, 255, 255), dst);			//dark skin
//	cvErode(dst, dst, 0, 3);
//	cvDilate(dst, dst, 0, 4);
//}

//////////////////// DETECT HAND ////////////////////

void detect_hand(IplImage* src, IplImage* lower, IplImage* upper, IplImage* mask, IplImage* msk_bgsubtract, IplImage* msk_color, IplImage* msk_fg, IplImage* dst)
{
	IplImage* msk_small = cvCreateImage(cvSize(160, 120), 8, 1);

	// BACKGROUND SUBTRACTION 
	bg_subtraction(src, lower, upper, msk_bgsubtract);

	// COLOR SEGMENTATION
	skin_yuv_color(src, msk_color);
	//skin_hsv_color(src, msk_color);

	// HAND
	cvAnd(msk_bgsubtract, msk_color, mask);
	cvCopy(mask, msk_fg);

	// BLOB LABELING				
	cvResize(mask, msk_small);
	cvThreshold(msk_small, msk_small, 100, 255, CV_THRESH_BINARY);
	CBlobLabeling blob;
	blob.SetParam(msk_small, 40);					// area
	blob.DoLabeling();								// much time here
	blob.BlobSmallSizeConstraint(15, 15);			// size min
	blob.BlobBigSizeConstraint(150, 200);		    // size max
	int Hand_Blob_Index = 0;

	// only select right most blob
	for (int i = 0; i < blob.m_nBlobs; i++)
	{
		// draw on hand
		//cvRectangle(msk_fg, cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 2, (blob.m_recBlobs[Hand_Blob_Index].y) * 2), cvPoint((blob.m_recBlobs[Hand_Blob_Index].x + blob.m_recBlobs[Hand_Blob_Index].width) * 2, (blob.m_recBlobs[Hand_Blob_Index].y + blob.m_recBlobs[Hand_Blob_Index].height) * 2), color_white, 1);

		if (blob.m_recBlobs[Hand_Blob_Index].x < blob.m_recBlobs[i].x)
			Hand_Blob_Index = i;
	}

	// draw rectangle cover hand blob on frame
	//cvRectangle(frame, cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 4, (blob.m_recBlobs[Hand_Blob_Index].y) * 4), cvPoint((blob.m_recBlobs[Hand_Blob_Index].x + blob.m_recBlobs[Hand_Blob_Index].width) * 4, (blob.m_recBlobs[Hand_Blob_Index].y + blob.m_recBlobs[Hand_Blob_Index].height) * 4), CV_RGB(255, 0, 0), 1);

	if (blob.m_nBlobs)
	{
		//if (blob.m_recBlobs[Hand_Blob_Index].width>160)			blob.m_recBlobs[Hand_Blob_Index].width = 160;
		if (blob.m_recBlobs[Hand_Blob_Index].height>threshold_blob_height)		blob.m_recBlobs[Hand_Blob_Index].height = threshold_blob_height;
		// pt1 is the top left of the blob
		pt1 = cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 2, (blob.m_recBlobs[Hand_Blob_Index].y) * 2);
		int width_new = (blob.m_recBlobs[Hand_Blob_Index].width) * 2;
		int height_new = (blob.m_recBlobs[Hand_Blob_Index].height) * 2;

		IplImage* sub_skin_tmp = cvCreateImage(cvSize(width_new, height_new), 8, 1);
		IplImage* sub_skin = cvCreateImage(cvSize(width_new, height_new), 8, 1);
		sub_skin->origin = src->origin;

		cvSetImageROI(mask, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(mask, sub_skin, 0);
		//cvCopy(sub_skin, sub_skin_tmp);
		cvResetImageROI(mask);
		cvZero(mask);

		// DISTANT TRANSFORM
		IplImage* sub_skin_temp = cvCreateImage(cvSize(width_new, height_new), IPL_DEPTH_32F, 1);
		cvDistTransform(sub_skin, sub_skin_temp, CV_DIST_L2, 3);

		float max_DT = 0.0;
		pt2.x = pt2.y = 0;
		pt.x = pt.y = 0;

		float* data_sub_1 = (float*)sub_skin_temp->imageData;

		int sub_w = sub_skin_temp->width;
		int sub_h = sub_skin_temp->height;
		int sub_ws = sub_skin_temp->width;

		for (int j = 0; j<sub_h; j++)
		for (int i = 0; i<sub_w; i++)
		{
			// find the max distant transform
			if (data_sub_1[j*sub_ws + i] > max_DT)
			{
				max_DT = data_sub_1[j*sub_ws + i];
				// hand center
				pt.x = i;
				pt.y = j;
			}
		}

		//print DT
		//printf("DT = %d  ",(int)max_DT);

		// REMOVE WRIST 
		int dis_wrist = (int)(radius*max_DT*radius*max_DT);
		uchar* data_sub_2 = (uchar*)sub_skin->imageData;
		sub_ws = sub_skin->widthStep;

		for (int j = 0; j<sub_h; j++)
		for (int i = 0; i<sub_w; i++)
		{
			if (data_sub_2[j*sub_ws + i] == 255)
			{
				if (pt2.x == 0 && pt2.y == 0)
				{
					pt2.x = i;
					pt2.y = j;
				}

				if (j > pt.y)
				{
					if (((pt.x - i)*(pt.x - i) + (pt.y - j)*(pt.y - j)) > dis_wrist)
						data_sub_2[j*sub_ws + i] = 0;
				}
			}
		}

		cvSetImageROI(mask, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(sub_skin, mask, 0);
		cvResetImageROI(mask);

		// show wrist
		//cvCircle(sub_skin_tmp, cvPoint(pt.x, pt.y), (int)(radius*max_DT), color_white, 3, 8, 0);
		//cvShowImage("Remove_Wrist", sub_skin_tmp);
		//cvShowImage("Only_Hand", sub_skin);

		// show Distant transform as a skeleton of hand
		//cvNormalize(sub_skin_temp, sub_skin_temp, 0.0, 1.0, NORM_MINMAX);
		//cvShowImage("Skeleton", sub_skin_temp);

		// pt is the center
		// pt1 is the top left of blob
		// pt2 is the farthest point
		pt2.x = pt1.x + pt2.x;
		pt2.y = pt1.y + pt2.y;
		pt.x = pt1.x + pt.x;
		pt.y = pt1.y + pt.y;
	}

	// CREATE HAND
	cvSetZero(dst);
	cvCopy(src, dst, mask);
}

//////////////////// DETECT FINGERS ////////////////////

void detect_fingers(IplImage* src, IplImage* dst)
{
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	CvSeq* maxitem = NULL;
	double area = 0, areamax = 0;
	CvPoint pt3;
	int maxn = 0;
	int Nc = cvFindContours(src, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST);
	int n = 0;
	CvFont font;

	//printf("Total Contours Detected: %d\n", Nc);

	if (Nc>0)
	{
		for (CvSeq* c = first_contour; c != NULL; c = c->h_next)
		{
			//cvCvtColor( src, dst, CV_GRAY2BGR );
			area = cvContourArea(c, CV_WHOLE_SEQ);
			if (area>areamax)
			{
				areamax = area;
				maxitem = c;
				maxn = n;
			}
			n++;
		}
		CvMemStorage* storage3 = cvCreateMemStorage(0);
		//if (maxitem) maxitem = cvApproxPoly( maxitem, sizeof(maxitem), storage3, CV_POLY_APPROX_DP, 3, 1 );  
		// the area should be larger than a fixed value
		if (areamax>1000)
		{
			maxitem = cvApproxPoly(maxitem, sizeof(CvContour), storage3, CV_POLY_APPROX_DP, 10, 1);
			CvPoint pt0;
			CvPoint end_pt;
			CvMemStorage* storage1 = cvCreateMemStorage(0);
			CvMemStorage* storage2 = cvCreateMemStorage(0);
			CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage1);
			CvSeq* hull;
			CvSeq* defects;
			for (int i = 0; i < maxitem->total; i++)
			{
				CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, maxitem, i);
				pt0.x = p->x;
				pt0.y = p->y;
				cvSeqPush(ptseq, &pt0);
			}
			hull = cvConvexHull2(ptseq, 0, CV_CLOCKWISE, 0);
			pt3.x = 0; pt3.y = 0;

			for (int x = 0; x<hull->total; x++)
			{
				CvPoint hull_pt = **CV_GET_SEQ_ELEM(CvPoint*, hull, x);
				if (pt3.x == 0 && pt3.y == 0)
				{
					pt3 = hull_pt;
					end_pt = pt3;
				}
				// draw on hand
				//cvLine(dst, pt3, hull_pt, CV_RGB(255, 0, 0), 1);               // DRAW RED LINE COVER HAND

				// draw on frame
				CvPoint pt3_f, hull_pt_f, end_pt_f;
				pt3_f.x = pt3.x * 2;
				pt3_f.y = pt3.y * 2;
				hull_pt_f.x = hull_pt.x * 2;
				hull_pt_f.y = hull_pt.y * 2;
				end_pt_f.x = end_pt.x * 2;
				end_pt_f.y = end_pt.y * 2;
				cvLine(dst, pt3_f, hull_pt_f, CV_RGB(255, 0, 0), 1);               // DRAW RED LINE COVER HAND

				pt3 = hull_pt;

				// draw on hand
				//if (x == hull->total - 1) cvLine(dst, hull_pt, end_pt, CV_RGB(255, 0, 0), 1);

				// draw on frame
				if (x == hull->total - 1) cvLine(dst, hull_pt_f, end_pt_f, CV_RGB(255, 0, 0), 1);

			}
			int hullcount = hull->total;
			defects = cvConvexityDefects(ptseq, hull, storage2);

			//printf(" defect no %d \n", defects->total);

			CvConvexityDefect* defectArray;
			num_fingers = 1;
			//int m_nomdef=0;
			// This cycle marks all defects of convexity of current contours.  
			for (; defects; defects = defects->h_next)
			{
				int nomdef = defects->total; // defect amount  
				//outlet_float( m_nomdef, nomdef );  
				//printf(" defect no %d \n",nomdef);
				if (nomdef == 0)
					continue;
				// Alloc memory for defect set.     
				//fprintf(stderr,"malloc\n");  
				defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
				// Get defect set.  
				//fprintf(stderr,"cvCvtSeqToArray\n");  
				cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
				// Draw marks for all defects.  
				for (int i = 0; i<nomdef; i++)
				{
					//	printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
					//if (defectArray[i].depth > 15)
					if (defectArray[i].depth > threshold_fingers)
					{
						num_fingers++;
						//printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);						
						//cvCircle( dst, *(defectArray[i].start), 5, CV_RGB(255,0,0), 2, 8,0);
						//cvCircle( dst, *(defectArray[i].depth_point), 5, CV_RGB(0,0,255), 2, 8,0);						
						//cvLine(dst, *(defectArray[i].start), *(defectArray[i].depth_point),CV_RGB(255,255,0),1, CV_AA, 0 );
						//cvLine(dst, *(defectArray[i].depth_point), *(defectArray[i].end),CV_RGB(255,255,0),1, CV_AA, 0 );  						
					}
				}
				// Free memory.
				free(defectArray);
			}
			// In case Num = 0
			double fDist = sqrt((double)((pt.x - pt2.x)*(pt.x - pt2.x) + (pt.y - pt2.y)*(pt.y - pt2.y)));
			//if (num_fingers == 1 && fDist < 42) num_fingers = 0;
			if (num_fingers == 1 && fDist < no_fingers) num_fingers = 0;
			//		printf("distant= %f\t",fDist );

			// draw on hand
			/*cvLine(dst, pt, pt2, CV_RGB(255, 255, 0), 2);
			cvCircle(dst, cvPoint(pt.x, pt.y), 4, CV_RGB(0, 255, 0), 2);
			cvCircle(dst, cvPoint(pt2.x, pt2.y), 4, CV_RGB(0, 0, 255), 2);*/

			// draw on frame
			CvPoint pt_f, pt2_f;
			pt_f.x = pt.x * 2; pt_f.y = pt.y * 2;
			pt2_f.x = pt2.x * 2; pt2_f.y = pt2.y * 2;
			cvLine(dst, pt_f, pt2_f, CV_RGB(255, 255, 0), 2);
			cvCircle(dst, cvPoint(pt_f.x, pt_f.y), 4, CV_RGB(0, 255, 0), 2);
			cvCircle(dst, cvPoint(pt2_f.x, pt2_f.y), 4, CV_RGB(0, 0, 255), 2);

			//printf("nomdef = %d, num_fingers = %d\n",nomdef,num_fingers);

			// print num of finger on the screen
			char txt[] = "0";
			txt[0] = '0' + num_fingers;
			cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 1.0, 1.0, 0, 2, CV_AA);
			cvPutText(dst, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));

			// release Storage
			cvReleaseMemStorage(&storage);
			cvReleaseMemStorage(&storage1);
			cvReleaseMemStorage(&storage2);
			cvReleaseMemStorage(&storage3);
		}
	}
}

//////////////////// RESET MASK ////////////////////

void reset_mask(IplImage* msk1, IplImage* msk2)
{
	cvZero(msk1);
	cvZero(msk2);
}

//////////////////// UPDATE SMA ////////////////////

//void update_SMA(int x, int y)
//{
//
//	LastX9 = LastX8;
//	LastX8 = LastX7;
//	LastX7 = LastX6;
//	LastX6 = LastX5;
//	LastX5 = x;
//	LastY9 = LastY8;
//	LastY8 = LastY7;
//	LastY7 = LastY6;
//	LastY6 = LastY5;
//	LastY5 = y;
//}

//////////////////// UPDATE EMA ////////////////////

void update_EMA(int x, int y)
{
	LastX4 = LastX3;
	LastX3 = LastX2;
	LastX2 = LastX1;
	LastX1 = LastX0;
	LastX0 = x;
	LastY4 = LastY3;
	LastY3 = LastY2;
	LastY2 = LastY1;
	LastY1 = LastY0;
	LastY0 = y;
}