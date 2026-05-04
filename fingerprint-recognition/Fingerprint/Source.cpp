#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "lib/binarisation/AdaptiveLocalThreshold.h"
#include "lib/optimisation/Ideka.h"
#include "lib/thinning/GuoHall.h"
#include "lib/extraction/CrossingNumber.h"

using namespace cv;
using namespace std;

int getMax(int x, int y)
{
	if (x<y) return y;
	return x;
}

int getMin(int x, int y)
{
	if (x<y) return x;
	return y;
}

vector<Minutiae> calMinutiae(Mat sourceImage)
{
	Mat img = sourceImage.clone();
	localThreshold::binarisation(img, 41, 56);

	cv::threshold(img, img, 50, 255, cv::THRESH_BINARY);
	Mat binImg = img.clone();
	ideka::binOptimisation(img);

	//skeletionizing
	cv::bitwise_not(img, img);    //Inverse for bit-operations
	GuoHall::thinning(img);
	cv::bitwise_not(img, img);

	//Minutiae-Extraction
	vector<Minutiae> minutiae;
	crossingNumber::getMinutiae(img, minutiae, 30);
	return minutiae;
}

void detectRoot(int& x, int& y, vector<Minutiae> m)
{
	int meanX = 0, meanY = 0;
	int tg = 100000000, tgX, tgY;
	for (int i = 0; i < m.size(); i++)
	{
		meanX += m[i].getLocX();
		meanY += m[i].getLocY();
	}
	meanX = meanX / m.size();
	meanY = meanY / m.size();
	for (int i = 0; i < m.size(); i++)
	{
		if (m[i].getType() == Minutiae::RIDGEENDING)
		{
			int d = m[i].distanceMean(meanX, meanY);
			if (d<tg)
			{
				tg = d;
				tgX = m[i].getLocX();
				tgY = m[i].getLocY();
			}
		}
	}
	x = tgX;
	y = tgY;
}

float calDistanceOfTwoPoint(Minutiae m1, Minutiae m2, int rootX1, int rootY1, int rootX2, int rootY2)
{
	float x1 = (float)m1.getLocX() - (float)rootX1;
	float y1 = (float)m1.getLocY() - (float)rootY1;
	float x2 = (float)m2.getLocX() - (float)rootX2;
	float y2 = (float)m2.getLocY() - (float)rootY2;
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

bool compare(vector<Minutiae> m1, vector<Minutiae> m2)
{

	float eDistance = 10;
	int meanX1 = 0, meanX2 = 0, meanY1 = 0, meanY2 = 0;
	int total = getMin(m1.size(), m2.size());
	detectRoot(meanX1, meanY1, m1);
	detectRoot(meanX2, meanY2, m2);
	int count = 0;
	for (int i = 0; i <m1.size(); i++)
	{
		int dem = 0;
		for (int j = 0; j < m2.size(); j++)
		{
			if ((calDistanceOfTwoPoint(m1[i], m2[j], meanX1, meanY1, meanX2, meanY2) <= eDistance) && (m1[i].getType() == m2[j].getType()))
			{
				dem++;
			}
		}
		if (dem != 0)
		{
			count++;
		}
	}
	float t = (float)count / (float)total * 100;
	//cout << "tile=" << t << endl;
	cout << "Result: Match " << t << " %" << endl;
	if (t>95)
	{
		return true;
	}
	return false;
}

bool compareFinger(Mat f1, Mat f2)
{
	vector<Minutiae> minutiae1 = calMinutiae(f1);
	vector<Minutiae> minutiae2 = calMinutiae(f2);
	return compare(minutiae1, minutiae2);
}

void main()
{
	int id = 16;
	int num = 8;
	bool exit_mode = false;
	cout << "****************************" << endl;
	cout << "**** Searching Database ****" << endl;
	cout << "****************************" << endl;
	for (int i = 1; i <= id; i++)
	{
		for (int j = 1; j <= num; j++)
		{
			char str1[30] = "";
			sprintf(str1, "database/%d_%d.png", i, j);
			Mat sourceImage1 = imread(str1, CV_LOAD_IMAGE_GRAYSCALE);
			Mat sourceImage2 = imread("object/2.png", CV_LOAD_IMAGE_GRAYSCALE);
			bool check = compareFinger(sourceImage1, sourceImage2);
			if (!check)
			{
				cout << "Fingerprint Missmatch!!!" << endl;
				cout << "Continue Searching......" << endl;
				cout << "************************" << endl;
			}
			else
			{
				char str2[30] = "";
				sprintf(str2, "database/%d.jpg", i);
				Mat avatar = imread(str2, CV_LOAD_IMAGE_UNCHANGED);
				imshow("Identify", avatar);
				imshow("Fingerprint", sourceImage2);
				imshow("Database", sourceImage1);
				cout << "Match Fingerprint!!!" << endl;
				char str3[50] = "";
				switch (i)
				{
				case 1:  sprintf(str3, "PERSON NUMBER 1"); break;
				case 2:  sprintf(str3, "PERSON NUMBER 2"); break;
				case 3:  sprintf(str3, "PERSON NUMBER 3"); break;
				case 4:  sprintf(str3, "PERSON NUMBER 4"); break;
				case 5:  sprintf(str3, "PERSON NUMBER 5"); break;
				case 6:  sprintf(str3, "PERSON NUMBER 6"); break;
				case 7:  sprintf(str3, "PERSON NUMBER 7"); break;
				case 8:  sprintf(str3, "PERSON NUMBER 8"); break;
				case 9:  sprintf(str3, "PERSON NUMBER 9"); break;
				case 10: sprintf(str3, "PERSON NUMBER 10"); break;
				case 11: sprintf(str3, "PERSON NUMBER 11"); break;
				case 12: sprintf(str3, "PERSON NUMBER 12"); break;
				case 13: sprintf(str3, "PERSON NUMBER 13"); break;
				case 14: sprintf(str3, "PERSON NUMBER 14"); break;
				case 15: sprintf(str3, "PERSON NUMBER 15"); break;
				case 16: sprintf(str3, "PERSON NUMBER 16"); break;
				}
				cout << "************************" << endl;
				cout << "Identify: " << str3 << endl;
				cout << "************************" << endl;
				exit_mode = true;
				waitKey();
				break;
			}
			if (exit_mode) break;
		}
		if (exit_mode) break;
	}
	if (!exit_mode)
	{
		cout << "Error!" << endl;
		cout << "Fingerprint is not on database!!! Please try another Fingerprint!!!" << endl;
		system("pause");
	}
}