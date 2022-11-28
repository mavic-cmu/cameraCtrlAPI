/*****************************************************************//**
 * \file   testCameraCtrlMain.cpp
 * \brief  main function to create 3D world to test camera control APIs
 *         Camera coordinate:
 *                          Front - Back :  X axis
 *                          Left  - Right:  Z axis
 *                          Up    - Down :  Y axis
 *                          Roll         :  rotate along X axis
 *                          Pitch        :  rotate along Z axis
 *                          Yaw          :  rotate along Y axis
 * \author chenw
 * \date   November 2022
 *********************************************************************/

#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Camera3D.h"
#include "CameraUIInterface.h"
#include "GraphicFont.h"

using namespace std;

void showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the screen:" << endl;
	cout << "    Click left button of mouse to change the Pitch and Yaw of the camera view" << endl;
	cout << "    Use mouse wheel to move forward to backword,(zoom in, zoom out)" << endl;
	cout << "    W / S: Move up and down along y axis" << endl;
	cout << "    A / D : Move left and right along z axis" << endl;
	cout << endl;

	cout << "    R : reset to inital position" << endl;
	cout << "    P : Play mode, to play according to camera keyframes" << endl;

	cout << "    SPACE : Save current camera position as key frame" << endl;
	cout << "    ALT : Delete last positon from key frames" << endl;

	cout << "    ESC : exit program" << endl;
}


int main(void)
{
	int mouseEvent = 0, leftButton = 0, middleButton = 0, rightButton = 0,
		screenX = 0, screenY = 0;

	bool terminate = false;
	Camera3D camera;
	CameraUserController cameraController;
	FsOpenWindow(16, 16, 1200, 800, 1);

	//initialize special fonts (after FsOpenWindow)
	ComicSansFont comicsans;
	stringstream coordStream;     // for displaying coordinates on screen

	bool isDisPlayMode = false;
	static int displayedFrameCnt = 0;
	vector<Campos> keyFrames;

	showMenu();
	while (!terminate)
	{
		FsPollDevice();
		// get mouse event
		mouseEvent = FsGetMouseEvent(leftButton, middleButton, rightButton, screenX, screenY);

		int wid, hei;
		FsGetWindowSize(wid, hei);

		int key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = true;
			break;
		case FSKEY_SPACE:
			cameraController.addCameraKeyFrame();
			cout << "Add one key frame" << endl;
			break;
		case FSKEY_ALT:
			cameraController.deleteCameraKeyFrame();
			cout << "delete one key frame" << endl;
			break;
		case FSKEY_R:
			// reset to initial camera pos
			cameraController.resetCameraInitPos();
			break;
		case FSKEY_G:
			// get and print key frames coordinates
			cameraController.camera.getCameraKeyFrames(keyFrames);
			for (auto frame : keyFrames) {
				cout << cameraController.camera.posToString(frame) << endl;
			}
			break;
		case FSKEY_P:
			// play mode, to play viewer based on keyFrames
			isDisPlayMode = !isDisPlayMode;
			cameraController.camera.getCameraKeyFrames(keyFrames);
			cout << " isDisPlayMode " << isDisPlayMode;
			break;
		}

		// use mouse wheel to move forward and backward
		if (key == FSKEY_WHEELUP) {
			cameraController.onKeyBoardPress(MOVE_FORWARD);
		}
		if (key == FSKEY_WHEELDOWN) {
			cameraController.onKeyBoardPress(MOVE_BACKWARD);
		}

		// use W S to move up and down
		if (FsGetKeyState(FSKEY_W)) {
			cameraController.onKeyBoardPress(MOVE_UP);
		}
		if (FsGetKeyState(FSKEY_S)) {
			cameraController.onKeyBoardPress(MOVE_DOWN);
		}

		// use A D  to move left and right
		if (FsGetKeyState(FSKEY_A)) {
			cameraController.onKeyBoardPress(MOVE_LEFT);
		}
		if (FsGetKeyState(FSKEY_D)) {
			cameraController.onKeyBoardPress(MOVE_RIGHT);
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, wid, hei);

		// Set up 3D drawing
		cameraController.camera.setUpCameraProjection();
		cameraController.camera.setUpCameraTransformation();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);

		// 3D drawing from here
		glColor3ub(0, 0, 255);

		// draw floor
		glBegin(GL_LINES);
		int x;
		for (x = -3000; x <= 3000; x += 50)
		{
			glVertex3i(x, 0, -3000);
			glVertex3i(x, 0, 3000);
			glVertex3i(-3000, 0, x);
			glVertex3i(3000, 0, x);
		}
		glEnd();

		// draw some boxes/buildings
		glColor3ub(120, 120, 120);
		DrawingUtilNG::drawCube({ 110, 0, 120 }, { 140, 10, 140 });
		glColor3ub(120, 255, 120);
		DrawingUtilNG::drawCube({ 80, 0, -70 }, { 90, 15, -30 }, true);

		cameraController.drawCameraKeyFrame();

		// draw axes (x is red, y is green, z is blue, like in all drawing software)
		glLineWidth(4);
		glBegin(GL_LINES);

		glColor3ub(255, 0, 0);
		glVertex3i(-500, 0, 0);
		glVertex3i(500, 0, 0);

		glColor3ub(0, 255, 0);
		glVertex3i(0, -500, 0);
		glVertex3i(0, 500, 0);

		glColor3ub(0, 0, 255);
		glVertex3i(0, 0, -500);
		glVertex3i(0, 0, 500);

		glEnd();
		glLineWidth(1);

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		comicsans.setColorHSV(0, 1, 1);
		comicsans.drawText("Testing Camera control APIs!", 10, 60, .25);
		std::string data;
		comicsans.drawText(cameraController.getCurrCameraParameterString(), 10, 95, .15);

		//display coords of mouse
		if (leftButton && mouseEvent == FSMOUSEEVENT_MOVE) { // write coords on screen if left button is held down
			coordStream.str("");  // reset stream
			coordStream.precision(4);
			coordStream << " (" << screenX << ", " << screenY << ")";
			comicsans.setColorHSV(0, 1, 1);
			comicsans.drawText(coordStream.str().c_str(), screenX, screenY - 3, .10);
			cameraController.onMouseClick(screenX, screenY);
		}

		//play mode
		if (isDisPlayMode && displayedFrameCnt < keyFrames.size()) {
			Campos frame = keyFrames.at(displayedFrameCnt);
			cameraController.setCameraToPos(frame);
			cout << cameraController.camera.posToString(frame) << endl;
		}
		else if (isDisPlayMode == false || displayedFrameCnt == keyFrames.size() - 1) {
			displayedFrameCnt = 0;
		}

		FsSwapBuffers();
		FsSleep(10);

		displayedFrameCnt++;
	}

	return 0;

}