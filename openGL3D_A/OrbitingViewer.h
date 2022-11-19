#pragma once
#include "Camera3D.h"

class OrbitingViewer
{
public:
	double h, p;
	double dist;
	double focusX, focusY, focusZ;

	OrbitingViewer();
	void initialize(void);
	void setUpCamera(Camera3D& camera);
};


