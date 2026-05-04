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

//////////////////// CODEBOOK METHOD ////////////////////
CvBGCodeBookModel* model = 0;
const int NCHANNELS = 3;
bool ch[NCHANNELS] = { true, true, true }; // This sets what channels should be adjusted for background bounds   

//////////////////// DECLARE ////////////////////
CvCapture* capture = 0;

CvSize size_img = cvSize(640, 480);
CvSize size_small = cvSize(320, 240);

IplImage* msk = cvCreateImage(size_small, 8, 1);
IplImage* skinImage = cvCreateImage(size_small, 8, 1);
IplImage* hand = cvCreateImage(size_small, 8, 3);
IplImage* frame = 0, *yuvImage = 0;
IplImage* ImaskCodeBook = 0, *ImaskCodeBookCC = 0;
IplImage* rawImage = cvCreateImage(size_small, 8, 3);

IplImage* ImaskCodeBookCC_temp = 0;

double radius = 1.6;

//////////////////// BLOB ////////////////////
//int sum_x, sum_y;
//int loop = 0;
//CvRect box, handmask;
// pt is the center, pt1 is the top left of blob, pt2 is the farthest point
CvPoint pt, pt0, pt1, pt2, Last_pt2 = cvPoint(0, 0);	
// max is max distant tranform value
float max_DT;											

//////////////////// INIT FONT ////////////////////
CvFont font;
CvScalar color;
CvPoint Eraser = cvPoint(200, 5), Yellow = cvPoint(300, 5), Mag = cvPoint(400, 5), Red = cvPoint(500, 5);

//////////////////// CONTROL INTERFACE ////////////////////
char LastNumFinger = 0, NumFinger = 0;

//////////////////// GUIDE ////////////////////
void guide(void)
{
	printf("=========================================================\n"
		"================ DAI HOC BACH KHOA TPHCM ================\n"
		"================== KHOA DIEN - DIEN TU ==================\n"
		"=================== LUAN VAN CAO HOC ====================\n"
		"=========================================================\n"
		"==================== HVTH: TRAN HOAN ====================\n"
		"===================== MSHV: 7140954 =====================\n"
		"=========================================================\n"
		);

	printf("\nINTERACTIVE PARAMETERS:\n"
		"\tESC,q,Q  - quit the program\n"
		"AVG PARAMETERS:\n"
		"\t-    - bump high threshold UP by 0.25\n"
		"\t=    - bump high threshold DOWN by 0.25\n"
		"\t[    - bump low threshold UP by 0.25\n"
		"\t]    - bump low threshold DOWN by 0.25\n"
		"CODEBOOK PARAMETERS:\n"
		"\ty,u,v- only adjust channel 0(y) or 1(u) or 2(v) respectively\n"
		"\ta	- adjust all 3 channels at once\n"
		"\tb	- adjust both 2 and 3 at once\n"
		"\ti,o	- bump upper threshold up,down by 1\n"
		"\tk,l	- bump lower threshold up,down by 1\n"
		"\tSPACE - reset the model\n"
		);
}

//////////////////// COLOR SEGMENTATION ////////////////////

void skin_yuv_color(IplImage* src, IplImage* dst)
{
	IplImage* yuv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, yuv, CV_GAUSSIAN, 5);
	cvCvtColor(yuv, yuv, CV_BGR2YCrCb);
	//cvInRangeS(yuv, cvScalar(30, 131, 50), cvScalar(245, 185, 120), dst);		// bright skin
	cvInRangeS(yuv, cvScalar(0, 131, 80), cvScalar(255, 185, 135), dst);		// dark skin
	cvErode(dst, dst, 0, 2);
	cvDilate(dst, dst, 0, 2);
	//cvShowImage("YUV", yuv);
}

void skin_hsv_color(IplImage* src, IplImage* dst)
{
	IplImage* hsv = cvCreateImage(size_small, 8, 3);
	cvSmooth(src, hsv, CV_GAUSSIAN, 5);
	cvCvtColor(hsv, hsv, CV_BGR2HSV);
	//cvInRangeS(hsv, cvScalar(0, 45, 0), cvScalar(20, 255, 255), dst);			//bright skin
	cvInRangeS(hsv, cvScalar(0, 45, 0), cvScalar(20, 255, 255), dst);			//dark skin
	cvErode(dst, dst, 0, 2);
	cvDilate(dst, dst, 0, 2);
	//cvShowImage("HSV", hsv);
}

//////////////////// DETECT HAND ////////////////////
void detect_hand(IplImage* src, IplImage* dst, IplImage* mask)
{
	IplImage* msk_small = cvCreateImage(cvSize(160, 120), 8, 1);			// blob labelling
	cvResize(mask, msk_small);												// to speed up
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
		//cvRectangle(frame,cvPoint( blob.m_recBlobs[i].x, blob.m_recBlobs[i].y),cvPoint( blob.m_recBlobs[i].x+ blob.m_recBlobs[i].width, blob.m_recBlobs[i].y +  blob.m_recBlobs[i].height),CV_RGB(0,0,255),1);
	}
	// draw rectangle cover hand blob
	cvRectangle(src, cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 4, (blob.m_recBlobs[Hand_Blob_Index].y) * 4), cvPoint((blob.m_recBlobs[Hand_Blob_Index].x + blob.m_recBlobs[Hand_Blob_Index].width) * 4, (blob.m_recBlobs[Hand_Blob_Index].y + blob.m_recBlobs[Hand_Blob_Index].height) * 4), CV_RGB(255, 0, 0), 1);

	if (blob.m_nBlobs)
	{
		if (blob.m_recBlobs[Hand_Blob_Index].height>45)		
			blob.m_recBlobs[Hand_Blob_Index].height = 40;
		//if (blob.m_recBlobs[Hand_Blob_Index].width>180)		blob.m_recBlobs[Hand_Blob_Index].width   = 180;
		// pt1 is the top left of the blob
		pt1 = cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 2, (blob.m_recBlobs[Hand_Blob_Index].y) * 2);	
		int width_new = (blob.m_recBlobs[Hand_Blob_Index].width) * 2;
		int height_new = (blob.m_recBlobs[Hand_Blob_Index].height) * 2;

		IplImage* sub_skin = cvCreateImage(cvSize(width_new, height_new), 8, 1);
		sub_skin->origin = src->origin;

		cvSetImageROI(mask, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(mask, sub_skin, 0);
		cvResetImageROI(mask);
		cvSetZero(mask);

		///////////////////////
		// DISTANT TRANSFORM //
		///////////////////////

		IplImage* sub_skin_temp = cvCreateImage(cvSize(width_new, height_new), IPL_DEPTH_32F, 1);
		// DT
		cvDistTransform(sub_skin, sub_skin_temp, CV_DIST_L2, 3);			

		max_DT = 0.0;
		pt2.x = pt2.y = 0;
		pt.x = pt.y = 0;

		float* data_sub = (float*)sub_skin_temp->imageData;

		int sub_w = sub_skin_temp->width;
		int sub_h = sub_skin_temp->height;
		// width step let it fault
		int sub_ws = sub_skin_temp->width;									

		for (int j = 0; j<sub_h; j++)
		for (int i = 0; i<sub_w; i++)
		{
			if (data_sub[j*sub_ws + i] > max_DT)							// find the max distant transform
			{
				max_DT = data_sub[j*sub_ws + i];
				pt.x = i;													// hand center
				pt.y = j;
			}
		}

		//print DT
		//printf("DT = %d  ",(int)max_DT);

		// remove wrist
		int Dis2wrist = (int)(radius*max_DT*radius*max_DT);
		uchar* data_sub1 = (uchar*)sub_skin->imageData;
		sub_ws = sub_skin->widthStep;
		for (int j = 0; j<sub_h; j++)
		for (int i = 0; i<sub_w; i++)
		{
			if (data_sub1[j*sub_ws + i] == 255)
			{
				if (pt2.x == 0 && pt2.y == 0)
				{
					pt2.x = i;
					pt2.y = j;
				}

				if (j > pt.y)
				{
					if (((pt.x - i)*(pt.x - i) + (pt.y - j)*(pt.y - j)) > Dis2wrist)
						data_sub1[j*sub_ws + i] = 0;
				}
			}
		}
		cvSetImageROI(mask, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(sub_skin, mask, 0);
		cvResetImageROI(mask);
		cvCircle(sub_skin, cvPoint(pt.x, pt.y), (int)(radius*max_DT), CV_RGB(255, 255, 255), 3, 8, 0);
		// Show Distant transform as a skeleton of hand
		//cvNormalize(sub_skin_temp, sub_skin_temp, 0.0, 1.0, NORM_MINMAX);
		//cvShowImage("Skeleton", sub_skin_temp);

		pt2.x = pt1.x + pt2.x;							// pt2 la diem xa nhat = ngon tay
		pt2.y = pt1.y + pt2.y;							// pt1 la dinh blob
		pt.x = pt1.x + pt.x;							// pt la tam ban tay
		pt.y = pt1.y + pt.y;
	}
	cvSetZero(dst);
	cvCopy(src, dst, mask);						// create hand
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
	
	printf( "Total Contours Detected: %d\n", Nc );
	
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
			
			printf(" defect no %d \n",defects->total);
			
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

/////////////////////////////////////////////////////////////////////////////////////////
////////                              MAIN PROGRAM                               ////////
/////////////////////////////////////////////////////////////////////////////////////////
void main()
{
	// hight priority class
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		

	const char* camera_name = "Tracking";
	const char* hand_name = "Hand";
	const char* bg_name = "Background Subtraction";
	const char* skin_name = "Skin Color";
	const char* codebook_name = "Foreground CodeBook";
	const char* codebook_comp = "CodeBook Comp";

	cvNamedWindow(camera_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(codebook_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(codebook_comp, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(skin_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(bg_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(hand_name, CV_WINDOW_AUTOSIZE);
	
	int c, n, nframes = 0;
	//int nframesToLearnBG = 300;
	int nframesToLearnBG = 60;
	model = cvCreateBGCodeBookModel();
	//Set color thresholds to default values   
	model->modMin[0] = 3;
	model->modMin[1] = model->modMin[2] = 3;
	model->modMax[0] = 10;
	model->modMax[1] = model->modMax[2] = 10;
	model->cbBounds[0] = model->cbBounds[1] = model->cbBounds[2] = 10;
	guide();
	
	capture = cvCaptureFromCAM(1);
	if (!capture)  printf("Could not initialize capturing...\n");

	

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////// MAIN PROCESSING LOOP ///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////// 

	while (1)
	{
		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);

		cvResize(frame, rawImage, 1);
		cvFlip(rawImage, rawImage, 1);

		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////// BACKGROUND - CODEBOOK //////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		++nframes;
		if (!rawImage)
			break;
		//First time:   
		if (nframes == 1 && rawImage)
		{
			// CODEBOOK METHOD ALLOCATION   
			yuvImage = cvCloneImage(rawImage);
			ImaskCodeBook = cvCreateImage(cvGetSize(rawImage), IPL_DEPTH_8U, 1);
			ImaskCodeBookCC = cvCreateImage(cvGetSize(rawImage), IPL_DEPTH_8U, 1);
			cvSet(ImaskCodeBook, cvScalar(255));
		}
		// If we've got an rawImage and are good to go:                   
		if (rawImage)
		{
			cvCvtColor(rawImage, yuvImage, CV_BGR2YCrCb);						//YUV For codebook method			
			//This is where we build our background model   
			if (nframes - 1 < nframesToLearnBG)
				cvBGCodeBookUpdate(model, yuvImage);
			if (nframes - 1 == nframesToLearnBG)
				cvBGCodeBookClearStale(model, model->t / 2);
			//Find the foreground if any   
			if (nframes - 1 >= nframesToLearnBG)
			{
				// Find foreground by codebook method   
				cvBGCodeBookDiff(model, yuvImage, ImaskCodeBook);
				// This part just to visualize bounding boxes and centers if desired   
				cvCopy(ImaskCodeBook, ImaskCodeBookCC);
				cvSegmentFGMask(ImaskCodeBookCC);
				cvErode(ImaskCodeBookCC, ImaskCodeBookCC, 0, 2);
				cvDilate(ImaskCodeBookCC, ImaskCodeBookCC, 0, 2);			// closing	

				/////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////// BACKGROUND - COLOR SEG /////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////////
				skin_yuv_color(rawImage, skinImage);						// yuv_color_segmentation
				//skin_hsv_color(rawImage, skinImage);						// hsv_color_segmentation
				cvAnd(ImaskCodeBookCC, skinImage, msk);

				/////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////// DETECT HAND - FINGERS //////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////////

				// use codebook & color segmentation
				detect_hand(rawImage, hand, msk);
				detect_fingers(msk, hand);

				// only codebook
				//ImaskCodeBookCC_temp = cvCloneImage(ImaskCodeBookCC);
				//detect_hand(rawImage, hand, ImaskCodeBookCC_temp);
				//detect_fingers(ImaskCodeBookCC_temp, hand);
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// DISPLAY ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////  
		cvShowImage(camera_name, rawImage);
		cvShowImage(codebook_name, ImaskCodeBook);
		cvShowImage(codebook_comp, ImaskCodeBookCC);
		cvShowImage(skin_name, skinImage);
		cvShowImage(bg_name, msk);
		cvShowImage(hand_name, hand);

		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////// USER INPUT: ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		c = cvWaitKey(10) & 0xFF;
		c = tolower(c);
		// End processing on ESC, q or Q   
		if (c == 27 || c == 'q')
			break;
		//Else check for user input   
		switch (c)
		{
		case ' ':
			cvBGCodeBookClearStale(model, 0);
			nframes = 0;
			break;
			//CODEBOOK PARAMS   
		case 'y': case '0':
		case 'u': case '1':
		case 'v': case '2':
		case 'a': case '3':
		case 'b':
			ch[0] = c == 'y' || c == '0' || c == 'a' || c == '3';
			ch[1] = c == 'u' || c == '1' || c == 'a' || c == '3' || c == 'b';
			ch[2] = c == 'v' || c == '2' || c == 'a' || c == '3' || c == 'b';
			printf("CodeBook YUV Channels active: %d, %d, %d\n", ch[0], ch[1], ch[2]);
			break;
		case 'i': //modify max classification bounds (max bound goes higher)   
		case 'o': //modify max classification bounds (max bound goes lower)   
		case 'k': //modify min classification bounds (min bound goes lower)   
		case 'l': //modify min classification bounds (min bound goes higher)   
		{
					  uchar* ptr = c == 'i' || c == 'o' ? model->modMax : model->modMin;
					  for (n = 0; n<NCHANNELS; n++)
					  {
						  if (ch[n])
						  {
							  int v = ptr[n] + (c == 'i' || c == 'l' ? 1 : -1);
							  ptr[n] = CV_CAST_8U(v);
						  }
						  printf("%d,", ptr[n]);
					  }
					  printf(" CodeBook %s Side\n", c == 'i' || c == 'o' ? "High" : "Low");
		}
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// RELEASE CAPTURE & IMAGE //////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	cvReleaseCapture(&capture);

	cvDestroyWindow(camera_name);
	cvDestroyWindow(codebook_name);
	cvDestroyWindow(codebook_comp);
	cvDestroyWindow(skin_name);
	cvDestroyWindow(bg_name);
	cvDestroyWindow(hand_name);
	return 0;
}