#include <math.h>
#include "Camera3D.h"
#include "fssimplewindow.h"

const double Camera3D::PI = 3.1415927;

Camera3D::Camera3D()
{
	initialize();
}

void Camera3D::initialize(void)
{

	initialCameraPos.x = 1;
	initialCameraPos.y = 5.0;
	initialCameraPos.z = 10.0;
	initialCameraPos.b = 0;
	initialCameraPos.p = 0;
	initialCameraPos.h = 0;

	currCameraPos = initialCameraPos;

	fov = PI / 6.0;  // 30 degree
	nearZ = 0.1;
	farZ = 400.0;
}

void Camera3D::setUpCameraProjection(void)
{
	int wid, hei;
	double aspect;

	FsGetWindowSize(wid, hei);
	aspect = (double)wid / (double)hei;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov * 180.0 / PI, aspect, nearZ, farZ);
}

void Camera3D::setUpCameraTransformation(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(-currCameraPos.b * 180.0 / PI, 0.0, 0.0, 1.0); // bank
	glRotated(-currCameraPos.p * 180.0 / PI, 1.0, 0.0, 0.0); // pitch
	glRotated(-currCameraPos.h * 180.0 / PI, 0.0, 1.0, 0.0); // heading
	glTranslated(-currCameraPos.x, -currCameraPos.y, -currCameraPos.z);
}

void Camera3D::getForwardVector(double& vx, double& vy, double& vz)
{
	vx = -cos(currCameraPos.p) * sin(currCameraPos.h);
	vy = sin(currCameraPos.p);
	vz = -cos(currCameraPos.p) * cos(currCameraPos.h);
}

void Camera3D::setInitCamraPos(double x, double y, double z, double roll, double pitch, double yaw)
{
	initialCameraPos.x = x;
	initialCameraPos.y = y;
	initialCameraPos.z = z;
	initialCameraPos.b = roll;
	initialCameraPos.p = pitch;
	initialCameraPos.h = yaw;
}

void Camera3D::resetCameraPos(void)
{
	currCameraPos = initialCameraPos;
}

void Camera3D::resetCameraPos(double maxBound)
{
	// calculate the initial camera position based on input clould point center

}

int Camera3D::setCameraPosition(Campos position)
{
	currCameraPos = position;
	return 0;
}

void Camera3D::changeCameraTransition(CamMoveDirct direction, float speed)
{
	double vx, vy, vz;
	getForwardVector(vx, vy, vz);

	switch (direction) {
	case MOVE_FORWARD:
		currCameraPos.x += vx * 1 + 0.1;
		currCameraPos.y += vy * 1 + 0.1;
		currCameraPos.z += vz * 1 + 0.1;
		break;
	case MOVE_BACKWARD:
		currCameraPos.x -= vx * 1 + 0.1;
		currCameraPos.y -= vy * 1 + 0.1;
		currCameraPos.z -= vz * 1 + 0.1;
		break;
	case MOVE_LEFT:
		currCameraPos.x += vx * 2 + 0.1;
		break;
	case MOVE_RIGHT:
		currCameraPos.x -= vx * 2 + 0.1;
		break;
	}
}

void Camera3D::changeCameraRotation(CamMoveDirct direction, float sign)
{
	switch (direction) {
	case MOVE_ROLL:
		currCameraPos.b += sign * (Camera3D::PI / 180.0);
		break;
	case MOVE_PITCH:
		currCameraPos.p += sign * (Camera3D::PI / 180.0 / 4);
		break;
	case MOVE_YAW:
		currCameraPos.h += sign * (Camera3D::PI / 180.0);
		break;
	}
}

int Camera3D::saveCurrentKeyFrame()
{
	keyFrames.push_back(currCameraPos);

	return 0;
}

int Camera3D::deleteKeyFrame()
{
	keyFrames.pop_back();
	return 0;
}

int Camera3D::getCameraKeyFrames(std::vector<Campos>& camerakeyFrames)
{
	camerakeyFrames = keyFrames;
	return 0;
}

int Camera3D::drawKeyFrameCamPos()
{
	//TODO draw sphere of cube based on current camera positon
	return 0;
}
