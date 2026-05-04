#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <time.h>   
#include <Windows.h>
#include <thread>
#include <stdio.h>
#include <future>

using namespace cv;
using namespace std;
double t_desire;

/*Setup serial port connection and needed variables*/
char WriteBuffer[] = "h";
//char ReadBuffer[] = "h";
DWORD transmit;
//DWORD receive;
HANDLE hSerial = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
void SendUART()
{
	
}

/*This function has a number input and string output*/
string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

/*This function gets called whenever a trackbar position is changed*/
void on_trackbar(int, void*) {}

int offset = 165;
int mask1_xL = 470, mask1_xR = 640, mask1_yT = 0, mask1_yB = 400;
int mask2_xL = 44, mask2_xR = 89, mask2_yT = 0, mask2_yB = 480;
int BLUR_SIZE1 = 10, BLUR_SIZE2 = 3;
int THRESHOLD1 = 25, THRESHOLD2 = 170;

void createTrackbars(){
	//create window for trackbars
	namedWindow("Trackbar", CV_WINDOW_FREERATIO);

	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar("Offset", "Trackbar", &offset, 300, on_trackbar);
	createTrackbar("mask1_xL", "Trackbar", &mask1_xL, 640, on_trackbar);
	createTrackbar("mask1_xR", "Trackbar", &mask1_xR, 640, on_trackbar);
	createTrackbar("mask1_yT", "Trackbar", &mask1_yT, 480, on_trackbar);
	createTrackbar("mask1_yB", "Trackbar", &mask1_yB, 480, on_trackbar);
	createTrackbar("mask2_xL", "Trackbar", &mask2_xL, 640, on_trackbar);
	createTrackbar("mask2_xR", "Trackbar", &mask2_xR, 640, on_trackbar);
	createTrackbar("mask2_yT", "Trackbar", &mask2_yT, 480, on_trackbar);
	createTrackbar("mask2_yB", "Trackbar", &mask2_yB, 480, on_trackbar);
	createTrackbar("Blur size 1", "Trackbar", &BLUR_SIZE1, 255, on_trackbar);
	createTrackbar("Blur size 2", "Trackbar", &BLUR_SIZE2, 255, on_trackbar);
	createTrackbar("Threshold 1", "Trackbar", &THRESHOLD1, 255, on_trackbar);
	createTrackbar("Threshold 2", "Trackbar", &THRESHOLD2, 255, on_trackbar);
}

int main()
{
	if (hSerial != INVALID_HANDLE_VALUE)
	{
		printf("Port opened! \n");

		DCB dcbSerialParams;
		GetCommState(hSerial, &dcbSerialParams);

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.Parity = NOPARITY;
		dcbSerialParams.StopBits = ONESTOPBIT;

		SetCommState(hSerial, &dcbSerialParams);
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Serial port doesn't exist! \n");
		}
		else if (GetLastError() == ERROR_FILE_EXISTS)
		{
			printf("Error while setting up serial port! \n Serial port already exist! \n");
		}
	}

	/*Define variables*/
	Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
	int xpos_cactus, ypos_cactus;
	int xpos_dinosaur, ypos_dinosaur;
	int rect1[2] = { 0, 0 };
	int rect2[2] = { 0, 0 };
	int rect3[2] = { 0, 0 };
	int rect4[2] = { 0, 0 };

	double delta_t2;
	double cactus_speed;

	bool Dinosaur_Detected = false;
	bool Catus_Detected = false;
	bool cactus_out = true;
	bool cactus_in = false;
	time_t t_cactus_in, t_cactus_out;


	Mat frame1, frame2;
	Mat grayImage1_1, grayImage2_2;
	Mat grayImage1, grayImage2;
	Mat diffImage;
	Mat thresholdImage1, thresholdImage2;
	Mat final_thresImage1, final_thresImage2;

	createTrackbars();
	VideoCapture cap(1);
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam. Press any key to exit" << endl;
		getchar();
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size: " << dWidth << " x " << dHeight << endl;
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

	while (1)
	{
		bool Success = cap.read(frame1);
		waitKey(1);
		cap.read(frame2);
		waitKey(1);

		if (!Success)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		//mask creation for Cactus
		Mat mask1 = Mat::zeros(Size(640, 480), CV_8UC1);
		mask1(Range(mask1_yT, mask1_yB), Range(mask1_xL, mask1_xR)) = 255;
		//mask creation for Dinosaur
		Mat mask2 = Mat::zeros(Size(640, 480), CV_8UC1);
		mask2(Range(mask2_yT, mask2_yB), Range(mask2_xL, mask2_xR)) = 255;

		cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
		cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);

		//histogram equalization
		//equalizeHist(grayImage1, grayImage1);
		//equalizeHist(grayImage2, grayImage2);

		absdiff(grayImage1, grayImage2, diffImage);
		//blur the image to get rid of the noise. This will output an intensity image
		blur(grayImage2, thresholdImage2, Size(BLUR_SIZE2, BLUR_SIZE2));
		blur(diffImage, thresholdImage1, Size(BLUR_SIZE1, BLUR_SIZE1));
		threshold(thresholdImage1, thresholdImage1, THRESHOLD1, 255, THRESH_BINARY);
		threshold(thresholdImage2, thresholdImage2, THRESHOLD2, 255, THRESH_BINARY);
		thresholdImage2 = 255 - thresholdImage2;

		//and with mask
		bitwise_and(thresholdImage1, mask1, final_thresImage1);
		bitwise_and(thresholdImage2, mask2, final_thresImage2);
		//imshow("Cactus Tracking", final_thresImage1);
		//imshow("Dinosaur Tracking", final_thresImage2);

		Mat temp1, temp2;
		final_thresImage1.copyTo(temp1);
		final_thresImage2.copyTo(temp2);

		vector< vector<Point> > contours_Cactus, contours_Dinosaur;
		vector<Vec4i> hierarchy1, hierarchy2;

		// retrieves external contours
		findContours(temp1, contours_Cactus, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		findContours(temp2, contours_Dinosaur, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		/*tracking Dinosaur*/
		if (contours_Dinosaur.size() > 0)
			Dinosaur_Detected = true;
		else
			Dinosaur_Detected = false;

		if (Dinosaur_Detected)
		{
			vector< vector<Point> > largestContourVec2;
			largestContourVec2.push_back(contours_Dinosaur.at(contours_Dinosaur.size() - 1));
			objectBoundingRectangle = boundingRect(largestContourVec2.at(0));
			xpos_dinosaur = objectBoundingRectangle.x + objectBoundingRectangle.width;
			ypos_dinosaur = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
			rect1[0] = (objectBoundingRectangle.x + objectBoundingRectangle.width), rect1[1] = objectBoundingRectangle.y;
			rect2[0] = objectBoundingRectangle.x, rect2[1] = (objectBoundingRectangle.y + objectBoundingRectangle.height);

			//draw the Dinosaur
			rectangle(frame2, Point(rect1[0], rect1[1]), Point(rect2[0], rect2[1]), Scalar(0, 0, 255), 2);
			circle(frame2, Point(xpos_dinosaur, ypos_dinosaur), 3, Scalar(0, 0, 255), 5);
			putText(frame2, "  (" + intToString(xpos_dinosaur) + "," + intToString(ypos_dinosaur) + ")", Point(xpos_dinosaur, ypos_dinosaur), 1, 1, Scalar(0, 0, 255), 1);
		}

		//tracking Cactus
		if (contours_Cactus.size() > 0)
		{
			Catus_Detected = true;
			if (cactus_out)
			{
				cactus_in = true;
				t_cactus_in = getTickCount();
				cactus_out = false;
			}
			if (Catus_Detected)
			{
				vector< vector<Point> > largestContourVec1;
				largestContourVec1.push_back(contours_Cactus.at(contours_Cactus.size() - 1));
				objectBoundingRectangle = boundingRect(largestContourVec1.at(0));
				xpos_cactus = objectBoundingRectangle.x + objectBoundingRectangle.width;
				ypos_cactus = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
				rect3[0] = (objectBoundingRectangle.x + objectBoundingRectangle.width), rect3[1] = objectBoundingRectangle.y;
				rect4[0] = objectBoundingRectangle.x, rect4[1] = (objectBoundingRectangle.y + objectBoundingRectangle.height);

				//draw the Cactus
				rectangle(frame2, Point(rect3[0], rect3[1]), Point(rect4[0], rect4[1]), Scalar(0, 255, 0), 2);
				int xpos_temp, ypos_temp;
				xpos_temp = objectBoundingRectangle.x;
				ypos_temp = ypos_cactus;
				circle(frame2, Point(xpos_temp, ypos_temp), 3, Scalar(0, 255, 0), 5);					
				putText(frame2, "  (" + intToString(xpos_cactus) + "," + intToString(ypos_cactus) + ")", Point(xpos_temp, (ypos_temp - 20)), 1, 1, Scalar(0, 0, 255), 1);
			}
		}
		else
		{
			Catus_Detected = false;
			if (cactus_in)
			{
				cactus_in = false;
				cactus_out = true;
				t_cactus_out = getTickCount();
				delta_t2 = (t_cactus_out - t_cactus_in) / getTickFrequency();
				cactus_speed = abs(mask1_xL - mask1_xR) / delta_t2;
				t_desire = (xpos_cactus - xpos_dinosaur - offset) / cactus_speed;
				auto f = std::async(std::launch::async, []
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(int(t_desire * 1000)));
					cout << "Jump" << endl;
					if (hSerial != INVALID_HANDLE_VALUE)
					{
						WriteFile(hSerial, WriteBuffer, strlen(WriteBuffer), &transmit, NULL);
					}
				});

			}
		}
		imshow("MyVideo", frame2);
	}
	CloseHandle(hSerial);
	printf("Port closed!\n");
	return 0;
}