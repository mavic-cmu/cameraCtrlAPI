#include <iostream>
#include <fstream>
#include <vector>
#include "plycpp.h"


struct Point3D {
	std::shared_ptr<plycpp::PropertyArray> x, y, z, nx, ny, nz;
	std::shared_ptr<plycpp::PropertyArray> r, g, b, alpha;
	int size;
};

class readPLY {

public:
	Point3D thePoint;

	void init(std::string file);

};
