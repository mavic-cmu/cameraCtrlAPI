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
#include <sstream>
#include <iostream>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Camera3D.h"
#include "CameraUIInterface.h"
#include "preProcess.h"
#include "GraphicFont.h"
#include "trajGenerator.h"

using namespace std;

void showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the screen:" << endl;
	cout << "    Click left button of mouse to change the Pitch and Yaw of the camera view" << endl;
	cout << "    Use mouse wheel to move forward to backword,(zoom in, zoom out) or:" << endl;
	cout << "    F / B forward and backword,(zoom in, zoom out)" << endl;
	cout << endl;
	cout << "    W / S: Move up and down along y axis" << endl;
	cout << "    A / D : Move left and right" << endl;
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
	Point3D thePoint;
	preProcess pre;
	readPLY data;

	Camera3D camera;
	CameraUserController cameraController;

	FsOpenWindow(16, 16, 1200, 800, 1);

	//initialize special fonts (after FsOpenWindow)
	ComicSansFont comicsans;
	stringstream coordStream;     // for displaying coordinates on screen
	trajGenerator traj;
	traj.setVelocity(10);
	bool isDisPlayMode = false;
	static int displayedFrameCnt = 0;
	vector<Campos> keyFrames;
	Campos currCamPos;
	Point currCamPoint;
	vector<Point> generatedTraj;

	data.init("flowers.ply");
	cout << endl;
	// Example how to access the data, first element
	std::cout << "x value of the first vertex element: " << data.thePoint.x->at<float>(0) << std::endl;
	std::cout << "y value of the first vertex element: " << data.thePoint.y->at<float>(0) << std::endl;
	std::cout << "z value of the first vertex element: " << data.thePoint.z->at<float>(0) << std::endl;
	std::cout << "nx value of the first vertex element: " << data.thePoint.nx->at<float>(0) << std::endl;
	std::cout << "ny value of the first vertex element: " << data.thePoint.ny->at<float>(0) << std::endl;
	std::cout << "nz value of the first vertex element: " << data.thePoint.nz->at<float>(0) << std::endl;
	std::cout << "r value of the first vertex element: " << static_cast<unsigned int>(data.thePoint.r->at<unsigned char>(0)) << std::endl;
	std::cout << "g value of the first vertex element: " << static_cast<unsigned int>(data.thePoint.g->at<unsigned char>(0)) << std::endl;
	std::cout << "b value of the first vertex element: " << static_cast<unsigned int>(data.thePoint.b->at<unsigned char>(0)) << std::endl;
	std::cout << "alpha value of the first vertex element: " << static_cast<unsigned int>(data.thePoint.alpha->at<unsigned char>(0)) << std::endl;
	std::cout << "vertex size: " << data.thePoint.size << std::endl;

	const int percentPrint = 5;
	int step = data.thePoint.size / (100 / percentPrint);
	int nextPrint = step;

	pre.convert3Dpoint(data);
	pre.getallvalue();
	pre.PointDownsize(0,4);

	for (int i = 0; i < data.thePoint.size; i++) {
		if (i >= nextPrint)
		{
			double percent = (100 * i) / data.thePoint.size;
			std::cout << "\r" << "paint PLY progress: " << std::string(percent / percentPrint, '|') << percent << "%";
			std::cout.flush();
			nextPrint += step;
		}
	}

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
			currCamPos = cameraController.getCurrCameraPos();
			currCamPoint = { {currCamPos.x, currCamPos.y, currCamPos.z}, currCamPos.roll / 45. * atan(1.), currCamPos.pitch / 45. * atan(1.), currCamPos.yaw / 45. * atan(1.) };
			traj.addKeyPoint(currCamPoint);
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
		if (key == FSKEY_WHEELUP || key == FSKEY_F) {
			cameraController.onKeyBoardPress(MOVE_FORWARD);
		}
		if (key == FSKEY_WHEELDOWN || key == FSKEY_B) {
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
		//glBegin(GL_LINES);
		//int x;
		//for (x = -3000; x <= 3000; x += 100)
		//{
		//	glVertex3i(x, -100, -3000);
		//	glVertex3i(x, -100, 3000);
		//	glVertex3i(-3000, -100, x);
		//	glVertex3i(3000, -100, x);
		//}
		//glEnd();

		pre.drawPoint(data);

		// draw some boxes/buildings
		//glColor3ub(120, 120, 120);
		//DrawingUtilNG::drawCube({ 110, 0, 120 }, { 140, 10, 140 });
		//glColor3ub(120, 255, 120);
		//DrawingUtilNG::drawCube({ 80, 0, -70 }, { 90, 15, -30 }, true);

		cameraController.drawCameraKeyFrame();

		// draw axes (x is red, y is green, z is blue, like in all drawing software)
		//glLineWidth(4);
		//glBegin(GL_LINES);

		//glColor3ub(255, 0, 0);
		//glVertex3i(-500, 0, 0);
		//glVertex3i(500, 0, 0);

		//glColor3ub(0, 255, 0);
		//glVertex3i(0, -500, 0);
		//glVertex3i(0, 500, 0);

		//glColor3ub(0, 0, 255);
		//glVertex3i(0, 0, -500);
		//glVertex3i(0, 0, 500);

		//glEnd();
		//glLineWidth(1);

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
			cameraController.onMouseClick(screenX, screenY, 0);
		}

		if (rightButton && mouseEvent == FSMOUSEEVENT_MOVE) { // write coords on screen if left button is held down
			coordStream.str("");  // reset stream
			coordStream.precision(4);
			coordStream << " (" << screenX << ", " << screenY << ")";
			comicsans.setColorHSV(0, 1, 1);
			comicsans.drawText(coordStream.str().c_str(), screenX, screenY - 3, .10);
			cameraController.onMouseClick(screenX, screenY, 1);
		}

		//play mode
		if (isDisPlayMode) {
			traj.genTraj(1);
			generatedTraj = traj.getTraj();
		}

		if (isDisPlayMode && displayedFrameCnt < generatedTraj.size()) {
			Campos frame = cameraController.getCurrCameraPos();
			frame.x = generatedTraj.at(displayedFrameCnt).pos.x;
			frame.y = generatedTraj.at(displayedFrameCnt).pos.y;
			frame.z = generatedTraj.at(displayedFrameCnt).pos.z;
			frame.yaw = generatedTraj.at(displayedFrameCnt).yaw / atan(1.) * 45.;
			frame.pitch = generatedTraj.at(displayedFrameCnt).pitch / atan(1.) * 45.;
			frame.roll = generatedTraj.at(displayedFrameCnt).roll / atan(1.) * 45.;
			cameraController.setCameraToPos(frame);
			cout << cameraController.camera.posToString(frame) << endl;

			if (displayedFrameCnt == generatedTraj.size() - 1) {
				isDisPlayMode = false;
			}
		}
		else if (isDisPlayMode == false || displayedFrameCnt == generatedTraj.size() - 1) {
			displayedFrameCnt = 0;

		}

		FsSwapBuffers();
		FsSleep(10);

		displayedFrameCnt++;
	}

	return 0;

}