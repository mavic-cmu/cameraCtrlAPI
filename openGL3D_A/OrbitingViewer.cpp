#include <math.h>
#include "fssimplewindow.h"
#include "OrbitingViewer.h"

OrbitingViewer::OrbitingViewer()
{
	initialize();
}

void OrbitingViewer::initialize(void)
{
	h = 0;
	p = 0;
	dist = 120.0;
	focusX = 0.0;
	focusY = 0.0;
	focusZ = 0.0;
}

void OrbitingViewer::setUpCamera(Camera3D& camera)
{
	camera.h = h;
	camera.p = p;
	camera.b = 0.0;

	double vx, vy, vz;
	camera.getForwardVector(vx, vy, vz);
	camera.x = focusX - vx * dist;
	camera.y = focusY - vy * dist;
	camera.z = focusZ - vz * dist;
}

