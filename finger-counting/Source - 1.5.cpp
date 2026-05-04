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
	int coordinates[18] = { 20, 30, 40, 30, 20, 30, 20, 30, 20, 40, 30, 40, 30, 40, 40, 50, 20, 40 };
	int num_coordinates = 18;
	int delta_x, delta_y, distance, angle;

	int draw_coordinates[100];
	int num_draw_coordinates = 0;

	for (int i = 0; i < num_coordinates - 3; i += 2)
	{
		delta_x = coordinates[i + 2] - coordinates[i];
		delta_y = coordinates[i + 3] - coordinates[i + 1];
		if ((delta_x != 0) || (delta_y != 0))
		{
			distance = round(sqrt(delta_x*delta_x + delta_y*delta_y));
			angle = round(atan2(delta_x, delta_y) * 180 / PI);
			cout << i << "," << delta_x << "," << delta_y << "," << distance << "," << angle << endl;

			draw_coordinates[num_draw_coordinates] = draw_coordinates[num_draw_coordinates + 2] = distance;
			draw_coordinates[num_draw_coordinates + 1] = draw_coordinates[num_draw_coordinates + 3] = angle;
			//draw_coordinates[num_draw_coordinates + 2] = distance;
			//draw_coordinates[num_draw_coordinates + 3] = angle;

			//cout << num_draw_coordinates << endl;
			//cout << endl << draw_coordinates[num_draw_coordinates] << "," << draw_coordinates[num_draw_coordinates+1] << endl;
			//cout << draw_coordinates[num_draw_coordinates + 2] << "," << draw_coordinates[num_draw_coordinates+3] << endl << endl;
			num_draw_coordinates += 4;
		}
	}
	for (int j = 0; j < num_draw_coordinates; j+=2)
	{
		cout << draw_coordinates[j] << "," << draw_coordinates[j + 1] << endl;
	}
	//x = -1;
	//y = 3;
	//result = atan2(x, y) * 180 / PI;
	//printf("The arc tangent for (x=%f, y=%f) is %f degrees\n", x, y, result);
	getchar();
	return 0;
}