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

#define PI 3.14159265

int main()
{
	float x, y, result;
	int angle;

	while (1)
	{
		cout << "x = ";
		cin >> x;
		cout << "y = ";
		cin >> y;
		
		result = atan2(x, y) * 180 / PI;
		angle = round(result);

		cout << "alpha = " << result << endl;
		cout << "angle = " << angle << endl << endl;
	}

}