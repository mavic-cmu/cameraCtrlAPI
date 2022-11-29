/*****************************************************************//**
 * \file   CameraUIInterface.cpp
 * \brief
 *
 * \author chenw
 * \date   November 2022
 *********************************************************************/
#include "CameraUIInterface.h"
#include <string>
#include <sstream>

using namespace std;

void CameraUserController::resetCameraInitPos(void) {
	camera.resetCameraPos();
}

void CameraUserController::setCameraToPos(Campos pos)
{
	camera.setCameraPosition(pos);
}

Campos CameraUserController::getCurrCameraPos(void)
{
	return camera.currCameraPos;
}

void CameraUserController::onKeyBoardPress(CamMoveDirct direction)
{
	double vx, vy, vz;

	switch (direction) {
	case ZOOM_IN:
	case MOVE_FORWARD:
		camera.getForwardVector(vx, vy, vz);
		camera.currCameraPos.x += vx * 1 + 0.1;
		camera.currCameraPos.y += vy * 1 + 0.1;
		camera.currCameraPos.z += vz * 1 + 0.1;
		break;
	case ZOOM_OUT:
	case MOVE_BACKWARD:
		camera.getForwardVector(vx, vy, vz);
		camera.currCameraPos.x -= vx * 1 + 0.1;
		camera.currCameraPos.y -= vy * 1 + 0.1;
		camera.currCameraPos.z -= vz * 1 + 0.1;
		break;
	case MOVE_RIGHT:
		camera.getHorizonMoveVector(vx, vy, vz);
		camera.currCameraPos.x += vx * 1 + 0.1;
		camera.currCameraPos.z += vz * 1 + 0.1;
		break;
	case MOVE_LEFT:
		camera.getHorizonMoveVector(vx, vy, vz);
		camera.currCameraPos.x -= vx * 1 + 0.1;
		camera.currCameraPos.z -= vz * 1 + 0.1;
		break;
	case MOVE_UP:
		camera.currCameraPos.y += 0.5;
		break;
	case MOVE_DOWN:
		camera.currCameraPos.y -= 0.5;
		break;
	}

	//cout << "vx " << vx << "vy " << vy << "vz " << vz << endl;
}

void CameraUserController::onMouseClick(int x, int y)
{
	float rollSign = 1.0;
	float pitchSign = 1.0;
	float yawSign = 1.0;
	bool changeRoll = false;
	bool changePitch = false;
	bool changeYaw = false;

	// to do calculate direction based on input
	if ((x - preMousePosX) > 1) {
		yawSign = 1.0;
		changeYaw = true;
	}
	else if ((preMousePosX - x) > 1) {
		yawSign = -1.0;
		changeYaw = true;
	}

	if ((y - preMousePosY) > 0) {
		pitchSign = 1.0;
		changePitch = true;
	}
	else if ((preMousePosY - y) > 0) {
		pitchSign = -1.0;
		changePitch = true;
	}

	if (changeRoll) {
		camera.currCameraPos.roll += changeRoll * (Camera3D::PI / 180.0);
	}

	if (changePitch) {
		camera.currCameraPos.pitch += pitchSign * (Camera3D::PI / 180.0);
	}

	if (changeYaw) {
		camera.currCameraPos.yaw += yawSign * (Camera3D::PI / 180.0);
	}

	preMousePosX = x;
	preMousePosY = y;
}

void CameraUserController::drawCameraKeyFrame(void)
{
	camera.drawKeyFrameCamPos();
}

void CameraUserController::addCameraKeyFrame(void)
{
	camera.saveCurrentKeyFrame();
}

void CameraUserController::deleteCameraKeyFrame(void)
{
	camera.deleteKeyFrame();
}

string CameraUserController::getCurrCameraParameterString(void)
{
	return camera.posToString(camera.currCameraPos);
}
