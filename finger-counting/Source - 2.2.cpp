#include <windows.h>
#include <ctype.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <cxcore.h>
#include <iostream>
#include <cv.h>
#include <ml.h>
#include "BlobLabeling.h"
#include "BlobLabeling.cpp"
#include <string>

#include "cvaux.h"
#include "cxmisc.h"
#include <stdlib.h>   
#include <ctype.h>   

using namespace cv;
using namespace std;

//////////////////// DEFINE ////////////////////
#define INIT_TIME 20
#define ZETA 10
#define radius 1.6

//////////////////// DECLARE ////////////////////
char c;
CvCapture* capture = 0;

//CvSize size_img = cvSize(640, 480);
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

//////////////////// BLOB ////////////////////
// pt is the center
// pt1 is the top left of blob
// pt2 is the farthest point
CvPoint pt, pt1, pt2 = cvPoint(0, 0);
// max is max distant tranform value
float max_DT;

//////////////////// INIT FONT ////////////////////
CvFont font;
CvScalar color;
CvPoint Eraser = cvPoint(200, 5), Yellow = cvPoint(300, 5), Mag = cvPoint(400, 5), Red = cvPoint(500, 5);

//////////////////// CONTROL INTERFACE ////////////////////
char LastNumFinger = 0, NumFinger = 0;

//////////////////// FUNCTION ////////////////////
void guide();
void bg_init(IplImage* dst1, IplImage* dst2);
void bg_subtraction(IplImage* src1, IplImage* src2, IplImage* src3, IplImage* dst);
void skin_yuv_color(IplImage* src, IplImage* dst);
void skin_hsv_color(IplImage* src, IplImage* dst);
void detect_hand(IplImage* src, IplImage* lower, IplImage* upper, IplImage* mask, IplImage* dst);
void detect_fingers(IplImage* src, IplImage* dst);


/////////////////////////////////////////////////////////////////////////////////////////
////////                              MAIN PROGRAM                               ////////
/////////////////////////////////////////////////////////////////////////////////////////
void main()
{
	// hight priority class
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	int hand_mode = 0;

	const char* camera_name = "Tracking";
	const char* fg_name = "Foreground";
	const char* hand_name = "Hand";

	const char* bg_name = "Background Subtraction";
	const char* skin_name = "Skin Color";

	cvNamedWindow(camera_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(fg_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(hand_name, CV_WINDOW_AUTOSIZE);

	cvNamedWindow(bg_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(skin_name, CV_WINDOW_AUTOSIZE);



	capture = cvCaptureFromCAM(0);
	if (!capture)  printf("Could not initialize capturing...\n");

	guide();

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////// MAIN PROCESSING LOOP ///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////// 

	while (1)
	{
		if (hand_mode == 1)
		{
			bg_init(lower, upper);
			hand_mode = 2;
		}

		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);

		cvFlip(frame, frame, 1);
		cvResize(frame, frame_small, 1);

		if (hand_mode == 2)
		{
			detect_hand(frame_small, lower, upper, msk, hand);
			detect_fingers(msk, hand);
			// convert x,y			
			pt.x = pt.x * 2;
			pt.y = pt.y * 2;
			pt1.x = pt1.x * 2;
			pt1.y = pt1.y * 2;
			pt2.x = pt2.x * 2;
			pt2.y = pt2.y * 2;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// DISPLAY ////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////// 
		cvShowImage(camera_name, frame);
		cvShowImage(fg_name, msk_fg);
		cvShowImage(hand_name, hand);

		cvShowImage(bg_name, msk_bgsubtract);
		cvShowImage(skin_name, msk_color);
		/////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////// USER INPUT ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		c = cvWaitKey(1);
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
	cvDestroyWindow(fg_name);
	cvDestroyWindow(hand_name);

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
	for (int j = 0; j<INIT_TIME; j++)
	{
		frame = cvQueryFrame(capture);
		if (frame == 0) j--;
		cvFlip(frame, frame, 1);
		cvResize(frame, tmp_3);
		cvConvert(tmp_3, tmp_4);
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
	cvConvert(src1, tmp);
	// display different
	cvInRange(tmp, src2, src3, dst);
	cvNot(dst, dst);
	cvErode(dst, dst, 0, 3);
	cvDilate(dst, dst, 0, 2);
}

//////////////////// COLOR SEGMENTATION ////////////////////

void skin_yuv_color(IplImage* src, IplImage* dst)
{
	IplImage* yuv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, yuv, CV_GAUSSIAN, 5);
	cvCvtColor(yuv, yuv, CV_BGR2YCrCb);
	//cvInRangeS(yuv, cvScalar(50, 131, 80), cvScalar(245, 185, 135), dst);		// bright skin
	//cvInRangeS(yuv, cvScalar(30, 131, 50), cvScalar(245, 185, 120), dst);		// bright skin
	cvInRangeS(yuv, cvScalar(0, 131, 80), cvScalar(255, 185, 135), dst);		// dark skin
	cvErode(dst, dst, 0, 3);
	cvDilate(dst, dst, 0, 4);
}

void skin_hsv_color(IplImage* src, IplImage* dst)
{
	IplImage* hsv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, hsv, CV_GAUSSIAN, 5);
	cvCvtColor(hsv, hsv, CV_BGR2HSV);
	//cvInRangeS(hsv, cvScalar(0, 45, 0), cvScalar(20, 255, 255), dst);			//bright skin
	cvInRangeS(hsv, cvScalar(0, 45, 0), cvScalar(20, 255, 255), dst);			//dark skin
	cvErode(dst, dst, 0, 3);
	cvDilate(dst, dst, 0, 4);
}

//////////////////// DETECT HAND ////////////////////
void detect_hand(IplImage* src, IplImage* lower, IplImage* upper, IplImage* mask, IplImage* dst)
{
	IplImage* msk_small = cvCreateImage(cvSize(160, 120), 8, 1);
	// BACKGROUND SUBTRACTION 
	bg_subtraction(src, lower, upper, msk_bgsubtract);
	// COLOR SEGMENTATION
	skin_yuv_color(src, msk_color);
	//skin_hsv_color(src, msk_color);
	// HAND
	cvAnd(msk_bgsubtract, msk_color, mask);
	//cvAnd(msk_bgsubtract, msk_color, msk_fg);
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
		if (blob.m_recBlobs[Hand_Blob_Index].x < blob.m_recBlobs[i].x)
			Hand_Blob_Index = i;
	}
	// draw rectangle cover hand blob
	cvRectangle(frame, cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 4, (blob.m_recBlobs[Hand_Blob_Index].y) * 4), cvPoint((blob.m_recBlobs[Hand_Blob_Index].x + blob.m_recBlobs[Hand_Blob_Index].width) * 4, (blob.m_recBlobs[Hand_Blob_Index].y + blob.m_recBlobs[Hand_Blob_Index].height) * 4), CV_RGB(255, 0, 0), 1);

	if (blob.m_nBlobs)
	{
		//if (blob.m_recBlobs[Hand_Blob_Index].width>180)	blob.m_recBlobs[Hand_Blob_Index].width = 180;
		if (blob.m_recBlobs[Hand_Blob_Index].height>45)		blob.m_recBlobs[Hand_Blob_Index].height = 40;
		// pt1 is the top left of the blob
		pt1 = cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 2, (blob.m_recBlobs[Hand_Blob_Index].y) * 2);
		int width_new = (blob.m_recBlobs[Hand_Blob_Index].width) * 2;
		int height_new = (blob.m_recBlobs[Hand_Blob_Index].height) * 2;

		IplImage* sub_skin = cvCreateImage(cvSize(width_new, height_new), 8, 1);
		sub_skin->origin = frame->origin;

		cvSetImageROI(mask, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(mask, sub_skin, 0);
		cvResetImageROI(mask);
		cvZero(mask);

		// DISTANT TRANSFORM
		IplImage* sub_skin_temp = cvCreateImage(cvSize(width_new, height_new), IPL_DEPTH_32F, 1);
		cvDistTransform(sub_skin, sub_skin_temp, CV_DIST_L2, 3);

		max_DT = 0.0;
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
		cvCircle(sub_skin, cvPoint(pt.x, pt.y), (int)(radius*max_DT), CV_RGB(255, 255, 255), 3, 8, 0);
		// show Distant transform as a skeleton of hand
		cvNormalize(sub_skin_temp, sub_skin_temp, 0.0, 1.0, NORM_MINMAX);
		cvShowImage("Skeleton", sub_skin_temp);

		// pt2 la diem xa nhat = ngon tay
		pt2.x = pt1.x + pt2.x;
		pt2.y = pt1.y + pt2.y;
		// pt1 la dinh blob
		// pt la tam ban tay
		pt.x = pt1.x + pt.x;
		pt.y = pt1.y + pt.y;
	}
	// CREATE HAND
	cvSetZero(dst);
	cvCopy(src, dst, mask);
}

//////////////////// DETECT FINGERS ////////////////////
void  detect_fingers(IplImage* src, IplImage* dst)
{
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	CvSeq* maxitem = NULL;
	double area = 0, areamax = 0;
	CvPoint pt3;
	int maxn = 0;
	int Nc = cvFindContours(src, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST);
	int n = 0;

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
				cvLine(dst, pt3, hull_pt, CV_RGB(255, 0, 0), 1);               // DRAW RED LINE COVER HAND
				pt3 = hull_pt;

				if (x == hull->total - 1) cvLine(dst, hull_pt, end_pt, CV_RGB(255, 0, 0), 1);
			}
			int hullcount = hull->total;
			defects = cvConvexityDefects(ptseq, hull, storage2);

			//printf(" defect no %d \n", defects->total);

			CvConvexityDefect* defectArray;
			NumFinger = 1;
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
					if (defectArray[i].depth > 15)
					{
						//		printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
						//		cvLine(dst, *(defectArray[i].start), *(defectArray[i].depth_point),CV_RGB(255,255,0),1, CV_AA, 0 );  
						//		cvCircle( dst, *(defectArray[i].depth_point), 5, CV_RGB(0,0,255), 2, 8,0);  
						//		cvCircle( dst, *(defectArray[i].start), 5, CV_RGB(255,0,0), 2, 8,0);  
						//		cvLine(dst, *(defectArray[i].depth_point), *(defectArray[i].end),CV_RGB(255,255,0),1, CV_AA, 0 );  
						NumFinger++;
					}
				}
				// Free memory.
				free(defectArray);
			}
			// In case Num=0
			double fDist = sqrt((double)((pt.x - pt2.x)*(pt.x - pt2.x) + (pt.y - pt2.y)*(pt.y - pt2.y)));
			if (NumFinger == 1 && fDist < 42) NumFinger = 0;
			//		printf("distant= %f\t",fDist );
			// draw 
			cvLine(dst, pt, pt2, CV_RGB(255, 255, 0), 2);
			cvCircle(dst, cvPoint(pt.x, pt.y), 4, CV_RGB(0, 255, 0), 2);
			cvCircle(dst, cvPoint(pt2.x, pt2.y), 4, CV_RGB(0, 0, 255), 2);

			//printf("nomdef = %d, numfinger = %d\n",nomdef,NumFinger);

			// print num of finger on the screen
			char txt[] = "0";
			txt[0] = '0' + NumFinger;
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