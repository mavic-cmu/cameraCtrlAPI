#include "readPLY.h"

void readPLY::init(std::string file)
{
	
	plycpp::PLYData data;
	plycpp::load("flowers.ply", data);

	auto vertexElement = data["vertex"];

	thePoint.x = vertexElement->properties["x"];
	thePoint.y = vertexElement->properties["y"];
	thePoint.z = vertexElement->properties["z"];

	thePoint.nx = vertexElement->properties["nx"];
	thePoint.ny = vertexElement->properties["ny"];
	thePoint.nz = vertexElement->properties["nz"];

	thePoint.r = vertexElement->properties["red"];
	thePoint.g = vertexElement->properties["green"];
	thePoint.b = vertexElement->properties["blue"];
	thePoint.alpha = vertexElement->properties["alpha"];

	thePoint.size = vertexElement->size();


}
