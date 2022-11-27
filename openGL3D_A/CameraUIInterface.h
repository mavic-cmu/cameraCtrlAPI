#pragma once
#include <iostream>
#include "Camera3D.h"

class CameraUserController {
private:
	int preMousePosX = 0, preMousePosY = 0;

public:
	Camera3D camera;
	void onKeyBoardPress(CamMoveDirct direction);
	void onMouseClick(int x, int y);
	void drawCameraKeyFrame(void);
	void addCameraKeyFrame(void);
	void deleteCameraKeyFrame(void);
	void resetCameraInitPos(void);
	std::string getCameraParameterString(void);

};
