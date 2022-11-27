#include <math.h>
#include <sstream>
#include <string>
#include "Camera3D.h"
#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include "yspng.h"
#include "GraphicFont.h"

const double Camera3D::PI = 3.1415927;

Camera3D::Camera3D()
{
	initialize();

	// load texture data
	YsRawPngDecoder pngTemp;
	//pngTemp.Decode("cameraImage.png");
	pngTemp.Decode("ComicSansFont01.png");

	glGenTextures(1, &cameraTextureID);		// Reserve one texture identifier, DON'T LOSE THE TEXTURE ID
	glBindTexture(GL_TEXTURE_2D, cameraTextureID);	// Making the texture identifier current (or bring it to the deck)

	// set up parameters for the current texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,	// Level of detail
		GL_RGBA,	// the "A" in RGBA will include the transparency
		pngTemp.wid,	// the image width and height
		pngTemp.hei,
		0,  // Border width, but not supported and needs to be 0.
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pngTemp.rgba);
}

void Camera3D::initialize(void)
{

	initialCameraPos.x = -100;
	initialCameraPos.y = 100.0;
	initialCameraPos.z = 10.0;
	initialCameraPos.roll = 0;
	initialCameraPos.pitch = (-30. / 180.) * PI;
	initialCameraPos.yaw = (-90. / 180.) * PI;
	initialCameraPos.nearZ = 10;
	initialCameraPos.farZ = 2000;

	currCameraPos = initialCameraPos;

	fov = PI / 3.0;  // 30 degree
	//nearZ = 0.1;
	//farZ = 400.0;
}

void Camera3D::setUpCameraProjection(void)
{
	int wid, hei;
	double aspect;

	FsGetWindowSize(wid, hei);
	aspect = (double)wid / (double)hei;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov * 180.0 / PI, aspect, currCameraPos.nearZ, currCameraPos.farZ);
}

void Camera3D::setUpCameraTransformation(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(-currCameraPos.roll * 180.0 / PI, 0.0, 0.0, 1.0); // bank
	glRotated(-currCameraPos.pitch * 180.0 / PI, 1.0, 0.0, 0.0); // pitch
	glRotated(-currCameraPos.yaw * 180.0 / PI, 0.0, 1.0, 0.0); // heading
	glTranslated(-currCameraPos.x, -currCameraPos.y, -currCameraPos.z);
}

void Camera3D::getForwardVector(double& vx, double& vy, double& vz)
{
	vx = -cos(currCameraPos.pitch) * sin(currCameraPos.yaw);
	vy = sin(currCameraPos.pitch);
	vz = -cos(currCameraPos.pitch) * cos(currCameraPos.yaw);
}

void Camera3D::getHorizonMoveVector(double& vx, double& vy, double& vz)
{
	vx = -cos(currCameraPos.pitch) * cos(currCameraPos.yaw);
	vy = sin(currCameraPos.pitch);
	vz = -cos(currCameraPos.pitch) * sin(currCameraPos.yaw);
}

void Camera3D::setInitCamraPos(double x, double y, double z, double roll, double pitch, double yaw)
{
	initialCameraPos.x = x;
	initialCameraPos.y = y;
	initialCameraPos.z = z;
	initialCameraPos.roll = roll;
	initialCameraPos.pitch = pitch;
	initialCameraPos.yaw = yaw;
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


	switch (direction) {
	case MOVE_FORWARD:
		getForwardVector(vx, vy, vz);
		currCameraPos.x += vx * 1 + 0.1;
		currCameraPos.y += vy * 1 + 0.1;
		currCameraPos.z += vz * 1 + 0.1;
		break;
	case MOVE_BACKWARD:
		getForwardVector(vx, vy, vz);
		currCameraPos.x -= vx * 1 + 0.1;
		currCameraPos.y -= vy * 1 + 0.1;
		currCameraPos.z -= vz * 1 + 0.1;
		break;
	case MOVE_LEFT:
		currCameraPos.x += vx * 2 + 0.1;
		currCameraPos.y -= vy * 2 + 0.1;
		break;
	case MOVE_RIGHT:
		currCameraPos.x -= vx * 2 + 0.1;
		currCameraPos.y += vy * 2 + 0.1;
		break;
	}
}

void Camera3D::changeCameraRotation(CamMoveDirct direction, float sign)
{
	switch (direction) {
	case MOVE_ROLL:
		currCameraPos.roll += sign * (Camera3D::PI / 180.0);
		break;
	case MOVE_PITCH:
		currCameraPos.pitch += sign * (Camera3D::PI / 180.0 / 4);
		break;
	case MOVE_YAW:
		currCameraPos.yaw += sign * (Camera3D::PI / 180.0);
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
	if (keyFrames.size() >= 1) {
		keyFrames.pop_back();
		return 0;
	}

	return -1;
}

int Camera3D::getCameraKeyFrames(std::vector<Campos>& camerakeyFrames)
{
	camerakeyFrames = keyFrames;
	return 0;
}

std::string Camera3D::posToString(Campos camera) {
	std::stringstream coordStream;
	coordStream.precision(4);
	coordStream << " x: " << camera.x
		<< " y: " << camera.y
		<< " z: " << camera.z
		<< " roll: " << camera.roll * 45. / atan(1.)
		<< " pitch: " << camera.pitch * 45. / atan(1.)
		<< " yaw: " << camera.yaw * 45. / atan(1.);

	return coordStream.str().c_str();
}

void Camera3D::drawCamera(Campos camera)
{
	const float cameraSize = 1.;
	double x1 = camera.x; double y1 = camera.y; double z1 = camera.z;
	double x2 = x1 + cameraSize; double y2 = y1 + cameraSize; double z2 = z1 + cameraSize;

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	glBegin(GL_QUADS);


	glColor3ub(0, 0, 0);  // blue face
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x1, y2, z1);

	glVertex3d(x1, y1, z2);
	glVertex3d(x2, y1, z2);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);

	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y1, z2);
	glVertex3d(x1, y1, z2);

	glVertex3d(x1, y2, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x1, y2, z2);


	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y2, z1);
	glVertex3d(x1, y2, z2);
	glVertex3d(x1, y1, z2);

	glVertex3d(x2, y1, z1);
	glVertex3d(x2, y2, z1);
	glVertex3d(x2, y2, z2);
	glVertex3d(x2, y1, z2);

	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);

	//ImpactFont aFont;
	////font tester (needs GraphicFont.h)

	//aFont.setColorHSV(30, 1, 1);
	////aFont->drawText3D("  ONE is the loneliest number",
	//aFont.drawText3D(" cam",
	//	{ (float)x2, (float)y1, (float)z2 }, { (float)x2, (float)y2, (float)z2 }, { (float)x2, (float)y1, (float)z1 }, .006, 0.);

	//aFont.drawText3D(" cam",
	//	{ (float)x2, (float)y1, (float)z1 }, { (float)x2, (float)y2, (float)z1 }, { (float)x1, (float)y1, (float)z1 }, .006, 0.);

	//aFont.drawText3D(" cam",
	//	{ (float)x1, (float)y1, (float)z1 }, { (float)x1, (float)y2, (float)z1 }, { (float)x1, (float)y1, (float)z2 }, .006, 0.);

	//aFont.drawText3D(" cam",
	//	{ (float)x1, (float)y1, (float)z2 }, { (float)x1, (float)y2, (float)z2 }, { (float)x2, (float)y1, (float)z2 }, .006, 0.);

	//aFont.drawText3D(" cam",
	//	{ (float)x1, (float)y2, (float)z1 }, { (float)x2, (float)y2, (float)z1 }, { (float)x1, (float)y2, (float)z2 }, .006, 0.);

	//aFont.drawText3D(" cam",
	//	{ (float)x2, (float)y1, (float)z2 }, { (float)x2, (float)y1, (float)z1 }, { (float)x1, (float)y1, (float)z2 }, .006, 0.);
}


int Camera3D::drawKeyFrameCamPos()
{
	if (keyFrames.size() > 0) {
		for (auto cam : keyFrames) {
			glColor3ub(120, 0, 0);
			drawCamera(cam);
		}
	}

	return 0;
}



