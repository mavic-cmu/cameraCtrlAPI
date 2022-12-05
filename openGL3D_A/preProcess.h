#include <iostream>
#include <fstream>
#include <vector>
#include <math.h> 
#include <cmath>
#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include "yspng.h"
#include "GraphicFont.h"
#include "readPLY.h"


using namespace std;


struct Point3D_singlepoint {
	float x, y, z;

};
struct Point3D_data {
	float x, y, z, nx, ny, nz;
	unsigned char r, g, b, alpha;
};



class preProcess {
public:
	int default_color;
	int maximum_density_color, maximum_density_gray;
	Point3D_singlepoint lowerBound = { -INFINITY, -INFINITY,-INFINITY };
	Point3D_singlepoint upperBound = { -INFINITY, -INFINITY,-INFINITY };
	Point3D_singlepoint midPoint = { -INFINITY, -INFINITY,-INFINITY };
	std::vector<Point3D_data>Converted_3Dpoint;
	std::vector<Point3D_data>Point_downsize;
	std::vector<Point3D_data>PointShow;

	void initPointcloud();
	// This function is to convert the point cloud information passed by readPLY function to struct Point3D_data
	void convert3Dpoint(readPLY& data);
	// This function is to get the Lower Boundary from the point cloud to make the initial position of the camera easy to adjust
	Point3D_singlepoint getLowerBound();
	// This function is to get the Upper Boundary from the point cloud to make the initial position of the camera easy to adjust
	Point3D_singlepoint getUpperBound();
	// This function is to find the center position of the point cloud to make the point cloud center itself on the middle of the drawing window.
	Point3D_singlepoint getCenter();
	// This function is to transfrom all the point cloud to the center position and move the mid point to [0,0,0].
	void reCenter();
	// Downsize the input_point cloud with user input or default value in order to make the pointcloud easy shown on the screen.
	void PointDownsize(int density, int ratio_set);
	// This function is to find the mid value of a vector after arrenge from large to small
	float findMid(vector <float> array);

	void drawPoint(readPLY& data);

	void sudoColor(string axis_str, bool if_one_color, int red, int green, int blue);

	void PointShow_reset();

	void getallvalue();
};