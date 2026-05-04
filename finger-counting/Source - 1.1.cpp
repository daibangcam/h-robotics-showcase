#include <windows.h>
#include <ctype.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <cxcore.h>
#include <iostream>
#include <cv.h>
#include <ml.h>		
#include "stdio.h" 
#include "BlobLabeling.h"
#include "BlobLabeling.cpp"
#include <string>

using namespace cv;

///////////////////////////////  	DEFINE    ///////////////////////////////
// initial background
#define INIT_TIME 20
#define ZETA 10
// handwriting
#define PI 3.1415926536
#define USE_OPENCV_GRID_SEARCH_AUTOTRAIN 0  // set to 0 to set SVM parameters manually
//#define NUMBER_OF_TRAINING_SAMPLES 100		// 4900 full
#define NUMBER_OF_TRAINING_SAMPLES 10330
//#define NUMBER_OF_TRAINING_SAMPLES 5000
//#define ATTRIBUTES_PER_SAMPLE 32
#define NUMBER_OF_CLASSES 36
// EMA
#define EMA_FLAG 1
#define TIME_PERIODS 5
// method for segment
//#define BACKGROUND 1					// 1: background averaging
// 0: color segmentation
////////////////////////////////    STRUCT     /////////////////////////////////
struct cp
{
	int x[66500];
	int y[66500];
	int num;
}
cp;

////////////////////////////////    DECLARE    ////////////////////////
INPUT i;
POINT pos_cursor;
CvCapture* capture = 0;
CvSize SIZEIMG = cvSize(640, 480);
CvSize SIZEIMG_small = cvSize(320, 240);
IplImage *frame = 0, *image = 0;
IplImage* FlvFrame = cvCreateImage(cvSize(1280, 480), 8, 3);
IplImage* frame_small = cvCreateImage(SIZEIMG_small, 8, 3);
IplImage* av = cvCreateImage(SIZEIMG_small, IPL_DEPTH_32F, 3);
IplImage* sgm = cvCreateImage(SIZEIMG_small, IPL_DEPTH_32F, 3);
IplImage* msk = cvCreateImage(SIZEIMG_small, 8, 1);
IplImage* msk_color = cvCreateImage(SIZEIMG_small, 8, 1);
IplImage* msk_bgSubtract = cvCreateImage(SIZEIMG_small, 8, 1);
IplImage* mask = cvCreateImage(SIZEIMG, 8, 3);
IplImage* draw_mask = cvCreateImage(SIZEIMG, 8, 3);
IplImage* ycc = cvCreateImage(SIZEIMG_small, 8, 3);
IplImage* Handwriting_mask = cvCreateImage(SIZEIMG, 8, 1);
IplImage* Handwriting_mask_1 = cvCreateImage(SIZEIMG_small, 8, 1);
IplImage* mask_han = cvCreateImage(SIZEIMG_small, 8, 1);
IplImage* hand = cvCreateImage(SIZEIMG_small, 8, 3);
IplImage* tmp = cvCreateImage(SIZEIMG_small, IPL_DEPTH_32F, 3);
IplImage* lower = cvCreateImage(SIZEIMG_small, IPL_DEPTH_32F, 3);
IplImage* upper = cvCreateImage(SIZEIMG_small, IPL_DEPTH_32F, 3);
// blob labelling
IplImage* msk_small = cvCreateImage(cvSize(160, 120), 8, 1);
// handwriting
IplImage* imagen;
IplImage* screenBuffer;
IplImage* scaledResult = cvCreateImage(cvSize(60, 60), 8, 1);
int drawing;
int r, last_x, last_y;
int VarContinue = 0;
IplImage* img = cvCreateImage(cvSize(60, 60), IPL_DEPTH_32F, 1);
// BLOB
int sum_x, sum_y;
int loop = 0;
CvRect box, handmask;
// pt is the center, pt1 is the top left of blob, pt2 is the farthest point
CvPoint pt, pt0, pt1, pt2, Last_pt2 = cvPoint(0, 0);
// max is max distant tranform value
float max_DT;
// group of similar character
struct gp
{
	bool Valid;
	int NumClasses;
	int Class[36];
};
char c;
//  time measure
int64 now, then;
double elapsed_seconds, tickspersecond = cvGetTickFrequency() * 1.0e6;
// font for display
CvFont font;
CvScalar color;
// control button
CvPoint Eraser = cvPoint(200, 5), Yellow = cvPoint(300, 5), Mag = cvPoint(400, 5), Red = cvPoint(500, 5);
// Control interface
char ButtonWidth = 80, ButtonHeight = 50, ButtonInit = 0;
char LastNumFinger = 0, NumFinger = 0;
char ClickOn;						// 1. Eraser
// 2. Yellow
// 3. Mag
// 4. Red
char OnButton = 0;
// EMA
int LastX0, LastX1, LastX2, LastX3, LastX4, LastX5, LastX6, LastX7, LastX8, LastX9;
int LastY0, LastY1, LastY2, LastY3, LastY4, LastY5, LastY6, LastY7, LastY8, LastY9;

//////////////////////////////   SUB FUNCTION    ////////////////////////////////
void  detect(IplImage* img_8uc1, IplImage* img_8uc3)
{
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	CvSeq* maxitem = NULL;
	double area = 0, areamax = 0;
	CvPoint pt3;
	int maxn = 0;
	int Nc = cvFindContours(img_8uc1, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST);
	int n = 0;
	//printf( "Total Contours Detected: %d\n", Nc );
	if (Nc>0)
	{
		for (CvSeq* c = first_contour; c != NULL; c = c->h_next)
		{
			//cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
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
				cvLine(hand, pt3, hull_pt, CV_RGB(255, 0, 0), 1);               // DRAW RED LINE COVER HAND
				pt3 = hull_pt;

				if (x == hull->total - 1) cvLine(hand, hull_pt, end_pt, CV_RGB(255, 0, 0), 1);
			}

			int hullcount = hull->total;

			defects = cvConvexityDefects(ptseq, hull, storage2);

			//printf(" defect no %d \n",defects->total);

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
						//		cvLine(img_8uc3, *(defectArray[i].start), *(defectArray[i].depth_point),CV_RGB(255,255,0),1, CV_AA, 0 );  
						//		cvCircle( img_8uc3, *(defectArray[i].depth_point), 5, CV_RGB(0,0,255), 2, 8,0);  
						//		cvCircle( img_8uc3, *(defectArray[i].start), 5, CV_RGB(255,0,0), 2, 8,0);  
						//		cvLine(img_8uc3, *(defectArray[i].depth_point), *(defectArray[i].end),CV_RGB(255,255,0),1, CV_AA, 0 );  
						NumFinger++;
					}
				}

				// Free memory.
				free(defectArray);
			}

			// In case Num=0
			double fDist = sqrt((double)((pt.x - pt2.x)*(pt.x - pt2.x) + (pt.y - pt2.y)*(pt.y - pt2.y)));
			if (NumFinger == 1 && fDist < 42) NumFinger = 0;

			//	printf("distant= %f\t",fDist );

			// draw 
			cvLine(hand, pt, pt2, CV_RGB(255, 255, 0), 2);
			cvCircle(hand, cvPoint(pt.x, pt.y), 4, CV_RGB(0, 255, 0), 2);
			cvCircle(hand, cvPoint(pt2.x, pt2.y), 4, CV_RGB(0, 0, 255), 2);

			//printf("nomdef = %d, numfinger = %d\n",nomdef,NumFinger);

			// print num of finger on the screen
			char txt[] = "0";
			txt[0] = '0' + NumFinger;

			cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 1.0, 1.0, 0, 2, CV_AA);
			cvPutText(img_8uc3, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));

			// release Storage
			cvReleaseMemStorage(&storage);
			cvReleaseMemStorage(&storage1);
			cvReleaseMemStorage(&storage2);
			cvReleaseMemStorage(&storage3);
		}
	}
}

///////////////////////////  INIT BACKGROUND SUBTRACTION MODEL  ////////////////////////
void BG_Init()
{
	cvSetZero(av);
	for (int i = 0; i < INIT_TIME; i++)
	{
		frame = cvQueryFrame(capture);
		if (frame == 0) i--;
		cvFlip(frame, frame, 1);

		cvResize(frame, frame_small);
		cvAcc(frame_small, av);
	}
	cvConvertScale(av, av, 1.0 / INIT_TIME);

	cvSetZero(sgm);
	for (int j = 0; j<INIT_TIME; j++)
	{
		frame = cvQueryFrame(capture);
		if (frame == 0) j--;
		cvFlip(frame, frame, 1);

		cvResize(frame, frame_small);
		cvConvert(frame_small, tmp);
		cvSub(tmp, av, tmp);
		cvPow(tmp, tmp, 2.0);
		cvPow(tmp, tmp, 0.5);
		cvAcc(tmp, sgm);
	}
	cvConvertScale(sgm, sgm, 1.0 / INIT_TIME);

	cvSub(av, sgm, lower);
	cvSubS(lower, cvScalarAll(ZETA), lower);

	cvAdd(av, sgm, upper);
	cvAddS(upper, cvScalarAll(ZETA), upper);
}

/////////////////////////////  COMPARE CURRENT IMAGE AND MODEL  ///////////////////////////
void Compare()
{

	// Background subtraction 
	// #if BACKGROUND
	cvConvert(frame_small, tmp);
	// display different
	cvInRange(tmp, lower, upper, msk_bgSubtract);				// cost much


	cvNot(msk_bgSubtract, msk_bgSubtract);

	cvErode(msk_bgSubtract, msk_bgSubtract, 0, 3);			// closing
	cvDilate(msk_bgSubtract, msk_bgSubtract, 0, 2);

	// update model    
	//	cvSub(tmp,av,tmp);
	//SKIP THIS
	//	cvPow(tmp,tmp,2.0);
	//	cvPow(tmp,tmp,0.5);
	//	cvRunningAvg(frame,av,0.001,msk);
	//	cvRunningAvg(tmp,sgm,0.001,msk);


	//	Color segmentation
	//#else

	cvSmooth(frame_small, ycc, CV_GAUSSIAN, 5);
	cvCvtColor(ycc, ycc, CV_BGR2YCrCb);
	//cvInRangeS(ycc, cvScalar(50, 131, 80), cvScalar(245, 185, 135), msk_color);			// skin color best
	cvInRangeS(ycc, cvScalar(0, 131, 80), cvScalar(255, 185, 135), msk_color);			// skin color best

	//cvCvtColor( frame, ycc, CV_BGR2YCrCb );
	//cvInRangeS( ycc, cvScalar(30,133,80), cvScalar(245,173,120), msk);			// skin color 1

	//cvCvtColor( frame, ycc, CV_BGR2HSV );
	//cvInRangeS( ycc, cvScalar(0,50,90,0), cvScalar(25,226,255,0), msk ); 		// skin color 2

	cvDilate(msk_color, msk_color, 0, 4);														// closing
	cvErode(msk_color, msk_color, 0, 3);

	cvAnd(msk_bgSubtract, msk_color, msk);

	//#endif


	/////////////////////////   BLOB Labeling	/////////////////////////////	
	// input	:	msk_small
	// output	:	Hand_Blob_index
	/////////////////////////////////////////////////////////////////////////

	cvResize(msk, msk_small);				// to speed up

	cvThreshold(msk_small, msk_small, 100, 255, CV_THRESH_BINARY);

	CBlobLabeling blob;
	blob.SetParam(msk_small, 100);					// area
	blob.DoLabeling();								// much time here
	blob.BlobSmallSizeConstraint(5, 5);			// size min
	blob.BlobBigSizeConstraint(150, 200);		    // size max
	//	cvZero(mask_han);
	int Hand_Blob_Index = 0;

	// only select right most blob

	for (int i = 0; i<blob.m_nBlobs; i++)
	{
		if (blob.m_recBlobs[Hand_Blob_Index].x < blob.m_recBlobs[i].x)		Hand_Blob_Index = i;
		//cvRectangle(frame,cvPoint( blob.m_recBlobs[i].x, blob.m_recBlobs[i].y),cvPoint( blob.m_recBlobs[i].x+ blob.m_recBlobs[i].width, blob.m_recBlobs[i].y +  blob.m_recBlobs[i].height),CV_RGB(0,0,255),1);
	}

	// draw rectangle cover hand blob
	cvRectangle(frame, cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 4, (blob.m_recBlobs[Hand_Blob_Index].y) * 4), cvPoint((blob.m_recBlobs[Hand_Blob_Index].x + blob.m_recBlobs[Hand_Blob_Index].width) * 4, (blob.m_recBlobs[Hand_Blob_Index].y + blob.m_recBlobs[Hand_Blob_Index].height) * 4), CV_RGB(255, 0, 0), 1);


	if (blob.m_nBlobs)
	{
		if (blob.m_recBlobs[Hand_Blob_Index].height > 45)		blob.m_recBlobs[Hand_Blob_Index].height = 40;
		//if (blob.m_recBlobs[Hand_Blob_Index].width > 180)		blob.m_recBlobs[Hand_Blob_Index].width  = 180;
		//if (blob.m_recBlobs[Hand_Blob_Index].width > 200)		blob.m_recBlobs[Hand_Blob_Index].width = 200;

		pt1 = cvPoint((blob.m_recBlobs[Hand_Blob_Index].x) * 2, (blob.m_recBlobs[Hand_Blob_Index].y) * 2);								// pt1 is the top left of the blob

		int width_new = (blob.m_recBlobs[Hand_Blob_Index].width) * 2;
		int height_new = (blob.m_recBlobs[Hand_Blob_Index].height) * 2;



		IplImage* sub_skin = cvCreateImage(cvSize(width_new, height_new), 8, 1);
		sub_skin->origin = frame->origin;

		cvSetImageROI(msk, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(msk, sub_skin, 0);
		cvResetImageROI(msk);
		cvZero(msk);

		//cvShowImage("Sub_Skin", sub_skin);

		//		cvDilate(sub_skin,sub_skin,0,2);		// opening
		//		cvErode(sub_skin,sub_skin,0,3);

		//		cvCopy (mask_han,msk,0);			// msk with right blob only


		///////////////////////
		// DISTANT TRANSFORM //
		///////////////////////

		IplImage* sub_skin_temp = cvCreateImage(cvSize(width_new, height_new), IPL_DEPTH_32F, 1);
		cvDistTransform(sub_skin, sub_skin_temp, CV_DIST_L2, 3);			// DT

		max_DT = 0.0;
		pt2.x = pt2.y = 0;
		pt.x = pt.y = 0;

		float* data_sub = (float*)sub_skin_temp->imageData;

		int sub_w = sub_skin_temp->width;
		int sub_h = sub_skin_temp->height;
		int sub_ws = sub_skin_temp->width;			// width step let it fault

		for (int j = 0; j<sub_h; j++)
		for (int i = 0; i<sub_w; i++)
		{
			if (data_sub[j*sub_ws + i] > max_DT)			// find the max distant transform
			{
				max_DT = data_sub[j*sub_ws + i];
				pt.x = i;									// hand centre
				pt.y = j;
			}
		}

		//print DT
		//printf("DT = %d  ",(int)max_DT);

		//		cvCircle(mask_han,cvPoint(pt1.x + pt.x, pt1.y + pt.y),(int)(1.6*max_DT),CV_RGB(255,255,255),3,8,0);

		// remove wrist
		int Dis2wrist = (int)(1.65*max_DT*1.65*max_DT);

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


		cvSetImageROI(msk, cvRect(pt1.x, pt1.y, width_new, height_new));
		cvCopy(sub_skin, msk, 0);
		cvResetImageROI(msk);


		cvCircle(sub_skin, cvPoint(pt.x, pt.y), (int)(1.65*max_DT), CV_RGB(255, 255, 255), 3, 8, 0);


		// Show Distant transform as a skeleton of hand
		cvNormalize(sub_skin_temp, sub_skin_temp, 0.0, 1.0, NORM_MINMAX);
		cvShowImage("Skeleton", sub_skin_temp);


		pt2.x = pt1.x + pt2.x;							// pt2 la diem xa nhat = ngon tay
		pt2.y = pt1.y + pt2.y;							// pt1 la dinh blob
		pt.x = pt1.x + pt.x;							// pt la tam ban tay
		pt.y = pt1.y + pt.y;


		/*		// UP HAND ONLY
		uchar* data_sub = (uchar*) sub_skin->imageData;
		int sub_w = sub_skin ->width;
		int sub_h = sub_skin ->height;
		int sub_ws = sub_skin ->widthStep;

		cp.num = 0;
		sum_x = 0;
		sum_y = 0;
		pt2.x = 0; pt2.y = 0;


		for (int j=0; j<sub_h;j++)
		for (int i=0; i<sub_w;i++)
		if(data_sub[j*sub_ws +i]==255)
		{
		if(pt2.x == 0 && pt2.y == 0)
		{
		pt2.x = i;
		pt2.y = j;
		}
		cp.x[cp.num] = i;
		sum_x += i;
		cp.y[cp.num] = j;
		sum_y += j;
		cp.num++;
		}

		pt.x = pt1.x+(int)(sum_x/cp.num);				// pt la tam ban tay
		pt.y = pt1.y+(int)(sum_y/cp.num);				// pt1 la dinh Blob

		pt2.x = pt1.x + pt2.x + 2;							// pt2 la diem xa nhat = ngon tay
		pt2.y = pt1.y + pt2.y;

		*/
	}

	//	cvCopy (mask_han,msk,0);
	cvZero(hand);
	cvCopy(frame_small, hand, msk);			// create hand

	//	detect(msk, hand);

}

void Update_EMA(int x, int y)
{
	LastX9 = LastX8;
	LastX8 = LastX7;
	LastX7 = LastX6;
	LastX6 = LastX5;
	LastX5 = LastX4;
	LastX4 = LastX3;
	LastX3 = LastX2;
	LastX2 = LastX1;
	LastX1 = LastX0;
	LastX0 = x;

	LastY9 = LastY8;
	LastY8 = LastY7;
	LastY7 = LastY6;
	LastY6 = LastY5;
	LastY5 = LastY4;
	LastY4 = LastY3;
	LastY3 = LastY2;
	LastY2 = LastY1;
	LastY1 = LastY0;
	LastY0 = y;
}

// Handwriting

/////////////////////////////	PREPROCESS START	////////////////////////////////////
void findX(IplImage* imgSrc, int* min, int* max){
	int i;
	int minFound = 0;
	CvMat data;
	// CvScalar maxVal=cvRealScalar(0);
	CvScalar val = cvRealScalar(0);
	for (i = 0; i< imgSrc->width; i++){
		cvGetCol(imgSrc, &data, i);
		val = cvSum(&data);
		if (val.val[0] > 0){
			*max = i;
			if (!minFound){
				*min = i;
				minFound = 1;
			}
		}
	}
}
void findY(IplImage* imgSrc, int* min, int* max){
	int i;
	int minFound = 0;
	CvMat data;
	//CvScalar maxVal=cvRealScalar(0);
	CvScalar val = cvRealScalar(0);
	for (i = 0; i< imgSrc->height; i++){
		cvGetRow(imgSrc, &data, i);
		val = cvSum(&data);
		if (val.val[0] > 0){
			*max = i;
			if (!minFound){
				*min = i;
				minFound = 1;
			}
		}
	}
}
CvRect findBB(IplImage* imgSrc){
	CvRect aux;
	int xmin, xmax, ymin, ymax;
	xmin = xmax = ymin = ymax = 0;

	findX(imgSrc, &xmin, &xmax);
	findY(imgSrc, &ymin, &ymax);

	aux = cvRect(xmin, ymin, xmax - xmin, ymax - ymin);
	return aux;
}
IplImage* preprocessing(IplImage* imgSrc, int new_width, int new_height){
	IplImage* result;
	CvMat data;
	CvMat dataA;
	CvRect bb;			//bounding box

	//	cvNot(imgSrc,imgSrc);				// Background: Black, Foreground: White

	// Moment  here
	//	imgSrc = SkewAngle(imgSrc);

	//Find bounding box
	bb = findBB(imgSrc);
	if (bb.x == 0 && bb.y == 0 && bb.width == 0 & bb.height == 0)
	{
		VarContinue = 1;

	}
	else {
		//Get bounding box data and no with aspect ratio, the x and y can be corrupted
		cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));
		int size = (bb.width>bb.height) ? bb.width : bb.height;
		result = cvCreateImage(cvSize(size, size), 8, 1);
		cvZero(result);
		//Copy de data in center of image
		int x = (int)floor((float)(size - bb.width) / 2.0f);
		int y = (int)floor((float)(size - bb.height) / 2.0f);
		cvGetSubRect(result, &dataA, cvRect(x, y, bb.width, bb.height));
		cvCopy(&data, &dataA, NULL);
		scaledResult = cvCreateImage(cvSize(new_width, new_height), 8, 1);
		cvResize(result, scaledResult, CV_INTER_NN);

		return scaledResult;

		//Return processed data
	}
	//	printf("bb=%d,%d,%d,%d",bb.x,bb.y,bb.width,bb.height);
	/*
	//Scale result
	scaledResult=cvCreateImage( cvSize( new_width, new_height ), 8, 1 );
	cvSetImageROI( imgSrc, bb );
	cvResize(imgSrc, scaledResult, CV_INTER_NN);
	cvResetImageROI( imgSrc);
	cvNot(scaledResult,scaledResult);
	//Return processed data
	return *scaledResult;
	*/
}
////////////////////////////////	 PREPROCESS END		 /////////////////////////////////////

////////////////////////////////	READ DATA   	/////////////////////////////////////

int read_data_from_csv(const char* filename, Mat data, Mat classes,
	int n_samples, int numofattr)
{
	float tmpf;

	// if we can't read the input file then return 0
	FILE* f = fopen(filename, "r");
	if (!f)
	{
		printf("ERROR: cannot read file %s\n", filename);
		return 0; // all not OK
	}

	// for each sample in the file

	for (int line = 0; line < n_samples; line++)
	{
		// for each attribute on the line in the file
		for (int attribute = 0; attribute < (numofattr + 1); attribute++)
		{
			if (attribute < numofattr)
			{

				// first 256 elements (0-255) in each line are the attributes
				fscanf(f, "%f,", &tmpf);
				data.at<float>(line, attribute) = tmpf;

			}
			else if (attribute == numofattr)
			{
				// attribute 256 is the class label {0 ... 9}
				fscanf(f, "%f,", &tmpf);
				classes.at<float>(line, 0) = tmpf;
			}
		}
	}

	fclose(f);

	return 1; // all OK
}

int read_data_for_group(const char* filename, Mat data, Mat classes,
	int n_samples, int numofattr, struct gp *grp)
{
	float tmpf[512];
	int ClassOfSample;

	// if we can't read the input file then return 0
	FILE* f = fopen(filename, "r");
	if (!f){
		printf("ERROR: cannot read file %s\n", filename);
		return 0; // all not OK
	}

	int count = 0;
	for (int line = 0; line < n_samples; line++)
	{
		// for each attribute on the line in the file
		for (int attribute = 0; attribute < (numofattr + 1); attribute++)
		{
			if (attribute < numofattr)
			{
				// first 256 elements (0-255) in each line are the attributes
				fscanf(f, "%f,", &tmpf[attribute]);
				// data.at<float>(line, attribute) = tmpf;
			}
			else if (attribute == numofattr)
			{
				// attribute 256 is the class label {0 ... 9}
				fscanf(f, "%d,", &ClassOfSample);
				// classes.at<float>(line, 0) = tmpf;
				for (int i = 0; i<grp->NumClasses; i++)
				{
					// is the sample in the group
					if (ClassOfSample == grp->Class[i])
					{
						classes.at<float>(count, 0) = ClassOfSample;
						for (int j = 0; j<numofattr; j++)
						{
							data.at<float>(count, j) = tmpf[j];
						}

						count++;
					}
				}
			}
		}
	}

	fclose(f);
	return 1; // all OK
}

/************************************************************************/
//					function and var for rebuild database				//
/************************************************************************/
// const
#define DATA_TO_TRAIN 1
#define DATA_TO_TEST  2
#define VER_1_UJI	  1
#define VER_2_UPV	  2

// global variable for data
char NameOfChar;
int ClassOfChar;
int TypeOfChar;
int VerOfChar;
int x[5][350];
int y[5][350];
int maxPoint[5];
int maxX = 0;
int minX = 0;
int maxY = 0;
int minY = 0;
int maxDistance = 0;

// var for findDP function
// x_subrectangle_ (1or2)
//  -----------
//  |  0 | 1  |
//  |----+----|
//  |  2 | 3  |
//  -----------
int tempx0, tempx0Flag;				// Flag indicate the status of coordinate tempx0
int tempy0, tempy0Flag;
int tempOutput[4][4];               // [numofrect][x1,y1,x2,y2]


// global variable for image
IplImage* scaledResultOriginal = cvCreateImage(cvSize(136, 204), 8, 1);
IplImage* scaledResult16x16 = cvCreateImage(cvSize(16, 16), 8, 1);
IplImage* scaledResult20x20 = cvCreateImage(cvSize(20, 20), 8, 1);

IplImage prs_image;

// sub_function
int inClass(char tempchar)
{
	switch (tempchar)
	{
	case '0':ClassOfChar = 24; break;
	case '1':ClassOfChar = 1; break;
	case '2':ClassOfChar = 2; break;
	case '3':ClassOfChar = 3; break;
	case '4':ClassOfChar = 4; break;
	case '5':ClassOfChar = 5; break;
	case '6':ClassOfChar = 6; break;
	case '7':ClassOfChar = 7; break;
	case '8':ClassOfChar = 8; break;
	case '9':ClassOfChar = 9; break;
	case 'a':ClassOfChar = 10; break;
	case 'b':ClassOfChar = 11; break;
	case 'c':ClassOfChar = 12; break;
	case 'd':ClassOfChar = 13; break;
	case 'e':ClassOfChar = 14; break;
	case 'f':ClassOfChar = 15; break;
	case 'g':ClassOfChar = 16; break;
	case 'h':ClassOfChar = 17; break;
	case 'i':ClassOfChar = 18; break;
	case 'j':ClassOfChar = 19; break;
	case 'k':ClassOfChar = 20; break;
	case 'l':ClassOfChar = 21; break;
	case 'm':ClassOfChar = 22; break;
	case 'n':ClassOfChar = 23; break;
	case 'o':ClassOfChar = 24; break;
	case 'p':ClassOfChar = 25; break;
	case 'q':ClassOfChar = 26; break;
	case 'r':ClassOfChar = 27; break;
	case 's':ClassOfChar = 28; break;
	case 't':ClassOfChar = 29; break;
	case 'u':ClassOfChar = 30; break;
	case 'v':ClassOfChar = 31; break;
	case 'w':ClassOfChar = 32; break;
	case 'x':ClassOfChar = 33; break;
	case 'y':ClassOfChar = 34; break;
	case 'z':ClassOfChar = 35; break;
	case 'A':ClassOfChar = 10; break;
	case 'B':ClassOfChar = 11; break;
	case 'C':ClassOfChar = 12; break;
	case 'D':ClassOfChar = 13; break;
	case 'E':ClassOfChar = 14; break;
	case 'F':ClassOfChar = 15; break;
	case 'G':ClassOfChar = 16; break;
	case 'H':ClassOfChar = 17; break;
	case 'I':ClassOfChar = 18; break;
	case 'J':ClassOfChar = 19; break;
	case 'K':ClassOfChar = 20; break;
	case 'L':ClassOfChar = 21; break;
	case 'M':ClassOfChar = 22; break;
	case 'N':ClassOfChar = 23; break;
	case 'O':ClassOfChar = 24; break;
	case 'P':ClassOfChar = 25; break;
	case 'Q':ClassOfChar = 26; break;
	case 'R':ClassOfChar = 27; break;
	case 'S':ClassOfChar = 28; break;
	case 'T':ClassOfChar = 29; break;
	case 'U':ClassOfChar = 30; break;
	case 'V':ClassOfChar = 31; break;
	case 'W':ClassOfChar = 32; break;
	case 'X':ClassOfChar = 33; break;
	case 'Y':ClassOfChar = 34; break;
	case 'Z':ClassOfChar = 35; break;
	default:ClassOfChar = 0;
	}
	return ClassOfChar;
}

int NormalizeCoordinate(int tmpCoordinate)
{
	// data from UPV have to divide by 1.52 in order to have the same coordinate with data from UJI
	if (VerOfChar == VER_2_UPV) tmpCoordinate = (int)tmpCoordinate / (1.52);
	// scale
	// return int (tmpCoordinate/2);
	// size=const
	return tmpCoordinate;
}


float area;
float halfarea;
float val;
float minDiff;
// DP and 
int DPLevel0[2];
int OutLevel0[4][4];
int DPLevel1[4][2];
int OutLevel1[16][4];
int DPLevel2[16][2];
int OutLevel2[64][4];
int DPLevel3[64][2];
int OutLevel3[256][4];
int DPLevel4[256][2];			 // stop at level 4 :)

void findDP(int x1, int y1, int x2, int y2)
{
	// init var
	area = 0;
	halfarea = 0.;
	minDiff = 3600.;
	// create temp MATRIX
	CvMat* dataTemp = cvCreateMat(x2 - x1 + 1, y2 - y1 + 1, CV_32FC1);
	CvMat* rowsTemp = cvCreateMat(x2 - x1 + 1, 1, CV_32FC1);
	CvMat* colsTemp = cvCreateMat(1, y2 - y1 + 1, CV_32FC1);

	cvGetSubRect(img, dataTemp, cvRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1));
	// find total number foreground pixels
	area = cvSum(dataTemp).val[0];

	//find tempy0
	for (int row = 0; row<dataTemp->rows; row++)
	{
		cvGetRow(dataTemp, rowsTemp, row);
		val = cvSum(rowsTemp).val[0];
		halfarea += val;
		// find min diff
		if (abs(area + val - 2 * halfarea)<minDiff)
		{
			minDiff = abs(area + val - (2 * halfarea));
			tempy0 = y1 + row;
			tempy0Flag = 1;
		}
		if (abs(area - 2 * halfarea)<minDiff)
		{
			minDiff = abs(area - 2 * halfarea);
			tempy0 = y1 + row;
			tempy0Flag = 0;
		}
	}

	// find tempx0
	halfarea = 0;
	minDiff = 3600.;
	for (int col = 0; col<dataTemp->cols; col++)
	{
		cvGetCol(dataTemp, colsTemp, col);
		val = cvSum(colsTemp).val[0];
		halfarea += val;
		// find min diff
		if (abs(area + val - 2 * halfarea)<minDiff)
		{
			minDiff = abs(area + val - (2 * halfarea));
			tempx0 = x1 + col;
			tempx0Flag = 1;
		}
		if (abs(area - 2 * halfarea)<minDiff)
		{
			minDiff = abs(area - 2 * halfarea);
			tempx0 = x1 + col;
			tempx0Flag = 0;
		}
	}

	// Coordinate before update flag
	// rect1
	tempOutput[0][0] = x1;		tempOutput[0][1] = y1;
	tempOutput[0][2] = tempx0;	tempOutput[0][3] = tempy0;
	// rect2
	tempOutput[1][0] = tempx0;	tempOutput[1][1] = y1;
	tempOutput[1][2] = x2;		tempOutput[1][3] = tempy0;
	// rect3
	tempOutput[2][0] = x1;		tempOutput[2][1] = tempy0;
	tempOutput[2][2] = tempx0;	tempOutput[2][3] = y2;
	// rect4
	tempOutput[3][0] = tempx0;	tempOutput[3][1] = tempy0;
	tempOutput[3][2] = x2;		tempOutput[3][3] = y2;

	// update flag
	if (tempy0Flag == 0)
	{
		tempOutput[2][1] += 1;
		tempOutput[3][1] += 1;
	}
	if (tempx0Flag == 0)
	{
		tempOutput[1][0] += 1;
		tempOutput[3][0] += 1;
	}

	// release mat
	cvReleaseMat(&dataTemp);
	cvReleaseMat(&rowsTemp);
	cvReleaseMat(&colsTemp);
}
///////////////////////////     MAIN     //////////////////////////////
void main()
{

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);				// hight priority class

	int x, y, DrawMode, c, MouseMode, MouseCoordinate;
	DrawMode = 0; MouseMode = 0;
	MouseCoordinate = 0;

	//char filename[10];
	//int filecount = 0;
	//char img_id[3];

	/********************  EMA VARIABLE  *********************/

#if(EMA_FLAG)

	double SMA_X, SMA_Y, EMA_X, EMA_Y;
	int EMA_Init = 0;
	SMA_X = SMA_Y = EMA_X = EMA_Y = 0.0;

#else

#endif

	/********************  END EMA VARIABLE  *********************/


	// save to video 
	//double fps = 24.0;
	//CvVideoWriter *writer = cvCreateVideoWriter("Output.avi",CV_FOURCC('F','L','V','1'),fps,cvSize(1280,480));

	const char* camera_name = "Tracking image";
	const char* backproject_name = "Foreground";
	const char* background_subtr_name = "Background Subtraction";
	const char* skin_name = "Skin Color";

	capture = cvCaptureFromCAM(0);

	if (!capture)  printf("Could not initialize capturing...\n");

	printf("Visual Mouse - by: HanO\n"
		"Hot keys: \n"
		"\tESC - Quit the program\n"
		"\tTab - Go to Draw mode\n"
		"\t's' - Go to Mouse mode\n");

	cvNamedWindow(camera_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(backproject_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Skeleton", CV_WINDOW_AUTOSIZE);
	cvNamedWindow(background_subtr_name, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(skin_name, CV_WINDOW_AUTOSIZE);

	// Handwriting
	//Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
	//Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES, 1, CV_32FC1);

	//////////////////////////////////////////////////////
	//					MAIN PROCESS					//
	//////////////////////////////////////////////////////
	while (1)
	{

		then = cvGetTickCount();

		if (DrawMode == 1)
		{
			BG_Init();
			DrawMode = 2;
			ButtonInit = 0;	//	CLEAR MASK
			//	CvVideoWriter *writer = cvCreateVideoWriter("Output.avi",CV_FOURCC('F','L','V','1'),fps,cvSize(1280,480));
#if (EMA_FLAG)
			LastX0 = LastX1 = LastX2 = LastX3 = LastX4 = LastX5 = LastX6 = LastX7 = LastX8 = LastX9 = 0;
			LastY0 = LastY1 = LastY2 = LastY3 = LastY4 = LastY5 = LastY6 = LastY7 = LastY8 = LastY9 = 0;
#else
#endif
		}

		do
		{
			frame = cvQueryFrame(capture);
		} while (!frame);

		//frame = cvQueryFrame(capture);

		/*now = cvGetTickCount();
		elapsed_seconds = (double)(now - then) / tickspersecond;
		printf("\ Get frame time:  %g \n ", elapsed_seconds);*/

		if (!frame) break;
		cvFlip(frame, frame, 1);
		cvResize(frame, frame_small);

		if (DrawMode == 2)
		{

			/*******************      AVERAGE BACKGROUND and SKIN COLOR		*********************/

			Compare();	   // pt2 is point of hand

			/*
			now = cvGetTickCount();
			elapsed_seconds = (double)(now - then) / tickspersecond;
			printf("\ Compare time:  %g \n ",elapsed_seconds);
			*/

			detect(msk, hand);

			/*
			now = cvGetTickCount();
			elapsed_seconds = (double)(now - then) / tickspersecond;
			printf("\ Detect time:  %g \n ",elapsed_seconds);
			*/

			// convert x,y			
			pt.x = pt.x * 2;
			pt.y = pt.y * 2;

			pt1.x = pt1.x * 2;
			pt1.y = pt1.y * 2;

			pt2.x = pt2.x * 2;
			pt2.y = pt2.y * 2;


			/***********************   EXPONENTIAL MOVING AVERAGE CALCULATION   *********************/
			// input pt2.x, pt2.y
			// output pt2.x, pt2.y after EMA calculation

#if (EMA_FLAG)
			if (EMA_Init<TIME_PERIODS)
			{
				Update_EMA(pt2.x, pt2.y);

				EMA_Init++;

				if (EMA_Init == TIME_PERIODS)
				{
					SMA_X = (LastX0 + LastX1 + LastX2 + LastX3 + LastX4 + LastX5 + LastX6 + LastX7 + LastX8 + LastX9) / TIME_PERIODS;
					SMA_Y = (LastY0 + LastY1 + LastY2 + LastY3 + LastY4 + LastY5 + LastY6 + LastY7 + LastY8 + LastY9) / TIME_PERIODS;
					EMA_X = SMA_X;
					EMA_Y = SMA_Y;
				}

			}
			else
			{

				EMA_X = (((double)pt2.x - EMA_X) * 2 / (TIME_PERIODS + 1)) + EMA_X;
				EMA_Y = (((double)pt2.y - EMA_Y) * 2 / (TIME_PERIODS + 1)) + EMA_Y;
				pt2.x = EMA_X;
				pt2.y = EMA_Y;
			}
#else
#endif


			///////////////////////		DRAW ON FRAME		//////////////////////////////
			//	DRAW EVERYTHING ON MASK AND DRAW MASK									//
			//	MASK : mask for draw button, and handwriting							//
			//	DRAW_MASK:	draw button													//
			//	HANDWRITING:	handwriting												//
			//////////////////////////////////////////////////////////////////////////////


			// start the control interface
			if (ButtonInit == 0)
			{
				cvZero(mask);
				cvZero(draw_mask);
				cvZero(Handwriting_mask);
				//	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6, 0, 1, CV_AA);
				//	cvRectangle(mask,Eraser,cvPoint(Eraser.x+ButtonWidth, Eraser.y+ButtonHeight),CV_RGB(255,255,255),CV_FILLED);
				cvRectangle(mask, Eraser, cvPoint(Eraser.x + ButtonWidth, Eraser.y + ButtonHeight), CV_RGB(255, 255, 255), 2);
				cvRectangle(draw_mask, Eraser, cvPoint(Eraser.x + ButtonWidth, Eraser.y + ButtonHeight), CV_RGB(255, 255, 255), 2);
				//	cvPutText(mask, "Eraser", cvPoint(Eraser.x+10, Eraser.y+30), &font, CV_RGB(0,0,0));

				cvRectangle(mask, Yellow, cvPoint(Yellow.x + ButtonWidth, Yellow.y + ButtonHeight), CV_RGB(255, 255, 0), CV_FILLED);
				cvRectangle(mask, Yellow, cvPoint(Yellow.x + ButtonWidth, Yellow.y + ButtonHeight), CV_RGB(255, 255, 255), 2);
				cvRectangle(draw_mask, Yellow, cvPoint(Yellow.x + ButtonWidth, Yellow.y + ButtonHeight), CV_RGB(255, 255, 255), CV_FILLED);
				// cvPutText(mask, "GREEN", cvPoint(Yellow.x+20, Yellow.y+30), &font, CV_RGB(255,255,255));

				cvRectangle(mask, Mag, cvPoint(Mag.x + ButtonWidth, Mag.y + ButtonHeight), CV_RGB(0, 255, 255), CV_FILLED);
				cvRectangle(mask, Mag, cvPoint(Mag.x + ButtonWidth, Mag.y + ButtonHeight), CV_RGB(255, 255, 255), 2);
				cvRectangle(draw_mask, Mag, cvPoint(Mag.x + ButtonWidth, Mag.y + ButtonHeight), CV_RGB(255, 255, 255), CV_FILLED);
				// cvPutText(mask, "BLUE", cvPoint(Mag.x+30, Mag.y+30), &font, CV_RGB(255,255,255));

				cvRectangle(mask, Red, cvPoint(Red.x + ButtonWidth, Red.y + ButtonHeight), CV_RGB(255, 0, 0), CV_FILLED);
				cvRectangle(mask, Red, cvPoint(Red.x + ButtonWidth, Red.y + ButtonHeight), CV_RGB(255, 255, 255), 2);
				cvRectangle(draw_mask, Red, cvPoint(Red.x + ButtonWidth, Red.y + ButtonHeight), CV_RGB(255, 255, 255), CV_FILLED);
				// cvPutText(mask, "BLUE", cvPoint(Mag.x+30, Mag.y+30), &font, CV_RGB(255,255,255));

				ButtonInit = 1;
				ClickOn = 0;
			}


			/*************************	 SELECT RECTANGLE    *************************/
			/*
			if(LastNumFinger==2 && NumFinger==1)
			{
			if(pt2.x>Eraser.x && pt2.x<(Eraser.x+ButtonWidth) && pt2.y>Eraser.y && pt2.y<(Eraser.y+ButtonHeight))
			{
			ClickOn = 1;
			color = CV_RGB(0,0,0);
			}

			if(pt2.x>Yellow.x && pt2.x<(Yellow.x+ButtonWidth) && pt2.y>Yellow.y && pt2.y<(Yellow.y+ButtonHeight))
			{
			ClickOn = 2;
			color = CV_RGB(255,255,0);
			}

			if(pt2.x>Mag.x && pt2.x<(Mag.x+ButtonWidth) && pt2.y>Mag.y && pt2.y<(Mag.y+ButtonHeight))
			{
			ClickOn = 3;
			color = CV_RGB(0,255,255);
			}

			if(pt2.x>Red.x && pt2.x<(Red.x+ButtonWidth) && pt2.y>Red.y && pt2.y<(Red.y+ButtonHeight))
			{
			ClickOn = 4;
			color = CV_RGB(255,0,0);
			}

			}		*/

			//  do NOT draw OnButton

			if (pt2.x>Eraser.x && pt2.x<(Eraser.x + ButtonWidth) && pt2.y>Eraser.y && pt2.y<(Eraser.y + ButtonHeight))
			{
				OnButton = 1;
				ClickOn = 1;
				color = CV_RGB(0, 0, 0);
			}
			else if (pt2.x>Yellow.x && pt2.x<(Yellow.x + ButtonWidth) && pt2.y>Yellow.y && pt2.y<(Yellow.y + ButtonHeight))
			{
				OnButton = 1;
				ClickOn = 2;
				color = CV_RGB(255, 255, 0);
			}
			else if (pt2.x>Mag.x && pt2.x<(Mag.x + ButtonWidth) && pt2.y>Mag.y && pt2.y<(Mag.y + ButtonHeight))
			{
				OnButton = 1;
				ClickOn = 3;
				color = CV_RGB(0, 255, 255);
			}
			else if (pt2.x>Red.x && pt2.x<(Red.x + ButtonWidth) && pt2.y>Red.y && pt2.y<(Red.y + ButtonHeight))
			{
				OnButton = 1;
				ClickOn = 4;
				color = CV_RGB(255, 0, 0);
			}
			else OnButton = 0;

			// auto set color and click to test 
			OnButton = 0;
			ClickOn = 4;
			color = CV_RGB(255, 0, 0);


			// limit distance between 2 frame for drawing
			int dis = (int)sqrt((double)((pt2.x - Last_pt2.x)*(pt2.x - Last_pt2.x) + (pt2.y - Last_pt2.y)*(pt2.y - Last_pt2.y)));


			//////////////////////////////////////////////////////////////
			//						DRAW ON MASK						//
			//////////////////////////////////////////////////////////////

			if (NumFinger == 1 && ClickOn > 1 && dis<60 && ClickOn != 1 && OnButton == 0)
			{
				cvLine(draw_mask, Last_pt2, pt2, CV_RGB(255, 255, 255), 15);				// for mask
				cvLine(Handwriting_mask, Last_pt2, pt2, CV_RGB(255, 255, 255), 15);		// for handwriting recognition		
				cvLine(mask, Last_pt2, pt2, color, 15);									// DRAW LINE			
			}
			if (NumFinger == 1 && ClickOn == 1 && dis<60 && OnButton == 0)
			{
				cvLine(draw_mask, Last_pt2, pt2, CV_RGB(0, 0, 0), 40);						//for mask
				cvLine(Handwriting_mask, Last_pt2, pt2, CV_RGB(0, 0, 0), 40);				// for handwriting recognition	
				cvLine(mask, Last_pt2, pt2, color, 40);									// ERASE LINE
			}


			// fingerwriting recognization
			if (LastNumFinger == 2 && NumFinger == 3)
			{
				cvResize(Handwriting_mask, Handwriting_mask_1);
				cvShowImage("ahihi", Handwriting_mask_1);}
			//	Mat data;
			//	Mat row_header;
			//	Mat rowLv1 = Mat(1, 8, CV_32FC1);
			//	Mat rowLv2 = Mat(1, 32, CV_32FC1);
			//	Mat rowLv3 = Mat(1, 128, CV_32FC1);
			//	Mat rowLv4 = Mat(1, 512, CV_32FC1);

			//	// resize
			//	cvResize(Handwriting_mask, Handwriting_mask_1);

			//	// normalize the character picture to 60x60
			//	scaledResult = preprocessing(Handwriting_mask_1, 60, 60);


			//	// calculate the coordinate of the Division Point at each level
			//	if (VarContinue == 1) VarContinue = 0;
			//	else
			//	{
			//		cvConvertScale(scaledResult, img, 1. / 255, 0);									// scale = 1/255
			//		//cvSaveImage("2.jpg",img);

			//		// LEVEL 0
			//		findDP(0, 0, 59, 59);
			//		DPLevel0[0] = tempx0;
			//		DPLevel0[1] = tempy0;
			//		for (int m = 0; m<4; m++)
			//		{
			//			for (int n = 0; n<4; n++)
			//			{
			//				OutLevel0[m][n] = tempOutput[m][n];
			//			}
			//		}

			//		// LEVEL 1
			//		for (int k = 0; k<4; k++)
			//		{
			//			findDP(OutLevel0[k][0], OutLevel0[k][1], OutLevel0[k][2], OutLevel0[k][3]);
			//			// get output for level 1
			//			DPLevel1[k][0] = tempx0;
			//			DPLevel1[k][1] = tempy0;
			//			for (int m = 0; m<4; m++)
			//			{
			//				for (int n = 0; n<4; n++)
			//				{
			//					OutLevel1[k * 4 + m][n] = tempOutput[m][n];
			//				}
			//			}

			//		}
			//		// matrix to test
			//		for (int k = 0; k<4; k++)
			//		{
			//			rowLv1.at<float>(0, k * 2) = DPLevel1[k][0] / 60.;
			//			rowLv1.at<float>(0, k * 2 + 1) = DPLevel1[k][1] / 60.;
			//		}


			//		// LEVEL 2
			//		for (int k = 0; k<16; k++)
			//		{
			//			findDP(OutLevel1[k][0], OutLevel1[k][1], OutLevel1[k][2], OutLevel1[k][3]);
			//			// get output for level 2
			//			DPLevel2[k][0] = tempx0;
			//			DPLevel2[k][1] = tempy0;
			//			for (int m = 0; m<4; m++)
			//			{
			//				for (int n = 0; n<4; n++)
			//				{
			//					OutLevel2[k * 4 + m][n] = tempOutput[m][n];
			//				}
			//			}
			//		}
			//		for (int k = 0; k<16; k++)
			//		{
			//			rowLv2.at<float>(0, k * 2) = DPLevel2[k][0] / 60.;
			//			rowLv2.at<float>(0, k * 2 + 1) = DPLevel2[k][1] / 60.;
			//		}


			//		// LEVEL 3
			//		for (int k = 0; k<64; k++)
			//		{
			//			//if (k==22) 
			//			findDP(OutLevel2[k][0], OutLevel2[k][1], OutLevel2[k][2], OutLevel2[k][3]);
			//			// get output for level 3
			//			DPLevel3[k][0] = tempx0;
			//			DPLevel3[k][1] = tempy0;
			//			for (int m = 0; m<4; m++)
			//			{
			//				for (int n = 0; n<4; n++)
			//				{
			//					OutLevel3[k * 4 + m][n] = tempOutput[m][n];
			//				}
			//			}
			//		}
			//		// matrix to test
			//		for (int k = 0; k<64; k++)
			//		{
			//			rowLv3.at<float>(0, k * 2) = DPLevel3[k][0] / 60.;
			//			rowLv3.at<float>(0, k * 2 + 1) = DPLevel3[k][1] / 60.;
			//		}

			//		// LEVEL 4
			//		for (int k = 0; k<256; k++)
			//		{
			//			findDP(OutLevel3[k][0], OutLevel3[k][1], OutLevel3[k][2], OutLevel3[k][3]);
			//			// get output for level 4
			//			DPLevel4[k][0] = tempx0;
			//			DPLevel4[k][1] = tempy0;
			//		}
			//		// matrix to test
			//		for (int k = 0; k<256; k++)
			//		{
			//			rowLv4.at<float>(0, k * 2) = DPLevel4[k][0] / 60.;
			//			rowLv4.at<float>(0, k * 2 + 1) = DPLevel4[k][1] / 60.;
			//		}


			//		// classify at stage 1	
			//		result = svm3->predict(rowLv3);

			//		// classify at stage 2
			//		switch ((int)result)
			//		{
			//		case 1:	case 18:
			//			result = G1_svm->predict(rowLv1); break;
			//		case 2:case 35:
			//			result = G2_svm->predict(rowLv2); break;
			//		case 3: case 5:
			//			result = G3_svm->predict(rowLv2); break;
			//		case 10: case 26:
			//			result = G4_svm->predict(rowLv2); break;
			//		case 13: case 24:
			//			result = G5_svm->predict(rowLv4); break;
			//		case 15: case 16:
			//			result = G6_svm->predict(rowLv4); break;
			//		case 17: case 20:
			//			result = G7_svm->predict(rowLv2); break;
			//		default: break;
			//		}


			//		// print result 
			//		if ((int)result >35)	{
			//			result = (int)(result + 29);
			//			printf("\n\tClass result: (Character %c)\n", (int)result);
			//		}
			//		else if ((int)result >9) {
			//			result = (int)(result + 55);
			//			printf("\n\tClass result: (Character %c)\n", (int)result);
			//		}
			//		else if ((int)result == 0){
			//			result = (int)(result + 24 + 55);
			//			printf("\n\tClass result: (Character %c)\n", (int)result);
			//		}
			//		else	{
			//			result = (int)(result + 48);
			//			printf("\n\tClass result: (Digit %c)\n", (int)result);
			//		}

					//// display on the screen
					//char txt[] = "0";
					//txt[0] = (int)result;

					//cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 2.0, 2.0, 0, 2, CV_AA);
					//cvPutText(frame, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));

				//	cvCopy(mask, frame, draw_mask);

				//	//filecount++;
				//	//itoa(filecount,img_id,10);
				//	//strcpy(filename,"");
				//	//strcat(filename, img_id);
				//	//strcat(filename, ".jpg");
				//	//cvSaveImage(filename,frame);

				//}

			

			// clear all when the number of finger == 5
			if (NumFinger == 5)
			{
				cvSetImageROI(draw_mask, cvRect(0, 60, 640, 480 - 60));
				cvZero(draw_mask);
				cvResetImageROI(draw_mask);
				cvSetImageROI(Handwriting_mask, cvRect(0, 60, 640, 480 - 60));
				cvZero(Handwriting_mask);

				cvZero(Handwriting_mask_1);
				cvResetImageROI(Handwriting_mask_1);

				cvResetImageROI(Handwriting_mask);
				cvSetImageROI(mask, cvRect(0, 60, 640, 480 - 60));
				cvZero(mask);
				cvResetImageROI(mask);
			}

			//// Number of Finger = 0 to choose another color to draw
			//if (NumFinger == 0)
			//{
			//	//	ClickOn = 0;
			//}


			//// display
			//char txt[] = "0";
			//txt[0] = (int)result;

			//cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 2.0, 2.0, 0, 2, CV_AA);
			//cvPutText(frame, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));


			// draw line for finger writing 
			cvLine(mask, cvPoint(300, 120), cvPoint(620, 120), CV_RGB(255, 255, 255), 2);
			cvRectangle(draw_mask, cvPoint(300, 120), cvPoint(620, 120), CV_RGB(255, 255, 255), 1);
			cvLine(mask, cvPoint(300, 170), cvPoint(620, 170), CV_RGB(255, 255, 255), 2);
			cvRectangle(draw_mask, cvPoint(300, 170), cvPoint(620, 170), CV_RGB(255, 255, 255), 1);
			cvLine(mask, cvPoint(300, 220), cvPoint(620, 220), CV_RGB(255, 255, 255), 2);
			cvRectangle(draw_mask, cvPoint(300, 220), cvPoint(620, 220), CV_RGB(255, 255, 255), 1);
			cvLine(mask, cvPoint(300, 270), cvPoint(620, 270), CV_RGB(255, 255, 255), 2);
			cvRectangle(draw_mask, cvPoint(300, 270), cvPoint(620, 270), CV_RGB(255, 255, 255), 1);
			cvLine(mask, cvPoint(300, 320), cvPoint(620, 320), CV_RGB(255, 255, 255), 2);
			cvRectangle(draw_mask, cvPoint(300, 320), cvPoint(620, 320), CV_RGB(255, 255, 255), 1);


			cvCopy(mask, frame, draw_mask);												// BUTTON + HANDWRITING

			/*			cvSetImageROI(FlvFrame, cvRect(0,0,640,480));
			cvCopy(frame,FlvFrame);
			cvResetImageROI(FlvFrame);
			cvSetImageROI(FlvFrame, cvRect(640,0,640,480));
			cvCopy(hand,FlvFrame);
			cvResetImageROI(FlvFrame);


			// 0.02s
			cvWriteFrame( writer, FlvFrame );							// WRITE TO FLV FILE
			*/
			Last_pt2 = pt2;
			LastNumFinger = NumFinger;
		}

		///////////////////////////		SAVE AND SHOW IMAGE		//////////////////////////

		cvShowImage(camera_name, frame);
		cvShowImage(backproject_name, hand);
		cvShowImage(background_subtr_name, msk_bgSubtract);
		cvShowImage(skin_name, msk_color);

		//// xdatlee
		//now = cvGetTickCount(); 
		//elapsed_seconds = (double)(now - then) / tickspersecond;
		//printf("show Image:  %g \n ",elapsed_seconds);

		////////////////////////////		WAIT KEY			///////////////////////////////

		c = cvWaitKey(1);
		if ((char)c == 9)
		{
			DrawMode = 1;			// convert mode
			loop = 0;
			MouseMode = 0;
		}
		else if ((char)c == 's')
		{
			DrawMode = 0;
			MouseMode ^= 1;
			loop = 0;
		}
		else if ((char)c == 'i')
		{
			MouseCoordinate ^= 1;
		}
		else if ((char)c == 27)
			break;
		else;

		now = cvGetTickCount();
		elapsed_seconds = (double)(now - then) / tickspersecond;
		printf("\ Final time:  %g \n ", elapsed_seconds);

	}
	////////////////////////////		RELEASE IMAGE AND WINDOW		////////////////////////////

	cvReleaseImage(&image);
	cvReleaseImage(&ycc);
	cvReleaseImage(&mask);
	cvReleaseImage(&mask_han);
	cvReleaseImage(&hand);

	cvReleaseImage(&av);
	cvReleaseImage(&sgm);
	cvReleaseImage(&msk);
	cvReleaseImage(&tmp);
	cvReleaseImage(&lower);
	cvReleaseImage(&upper);

	//	cvReleaseVideoWriter( &writer );						// RELEASE WRITER FOR SAVING VIDEO
	cvDestroyWindow(camera_name);
	cvDestroyWindow(backproject_name);
	cvDestroyWindow("Skeleton");
	cvDestroyWindow(skin_name);
	cvDestroyWindow(background_subtr_name);
	cvDestroyAllWindows();
	////////////////////////////////////////		END			////////////////////////////////////
}