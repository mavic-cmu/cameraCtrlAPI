#include <iostream>
#include <fstream>
#include <sstream>
#include "DrawingUtilNG.h"
#include "GraphicFont.h"
#include "StringPlus.h"
#include "ysglfontdata.h"
#include "yssimplesound.h"
#include "UIManager.h"


using namespace std;

UIManager::UIManager(int width, int height)
{
	winWidth = width;
	winHeight = height;
	FsOpenWindow(16, 16, winWidth, winHeight, 1, "3D Pointcloud Visualization Video Generator");

	//screenFont = new ComicSansFont;
	screenFont = new CourierNewFont;

	// set color and width
	glColor3b(255, 0, 0);  // red

	ButtonCollection* myButtons = new ButtonCollection; // put this AFTER FsOpenWindow()
	GraphicFont* buttonFont = new ArialFont;
	buttonFont->setColorRGB(0, 0, 0); // black

	addMainButtons(buttonFont, 10, 120);
	addAdvanceButtons(buttonFont, (winWidth - 120) / 2, winHeight / 2);
}

void UIManager::drawGridAndAxis(void) {

	//draw some boxes/buildings
	//glColor3ub(120, 120, 120);
	//DrawingUtilNG::drawCube({ 110, 0, 120 }, { 140, 10, 140 });
	//glColor3ub(120, 255, 120);
	//DrawingUtilNG::drawCube({ 80, 0, -70 }, { 90, 15, -30 }, true);

	// draw floor
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
	int x;
	for (x = -3000; x <= 3000; x += 100)
	{
		glVertex3i(x, -100, -3000);
		glVertex3i(x, -100, 3000);
		glVertex3i(-3000, -100, x);
		glVertex3i(3000, -100, x);
	}
	glEnd();

	//draw axes (x is red, y is green, z is blue, like in all drawing software)
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
}

void UIManager::showMenu()
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


void UIManager::paintEditIndicator()
{
	bool useAwful = false;

	if (!useAwful) {
		// draw yellow square all around, with the word "EDIT" at top right and
		// bottom left corners

		// box around (big yellow rectangle with slightly smaller white rectangle)
		glColor3ub(255, 255, 0); // yellow
		int frameThickness = 5;
		DrawingUtilNG::drawRectangle(frameThickness, frameThickness,
			winWidth - 2 * frameThickness - 1, winHeight - 2 * frameThickness - 1, true,
			winWidth, winHeight);

		// little boxes for EDIT label
		glColor3ub(255, 255, 0); // yellow
		int boxW = 40, boxH = 18;
		DrawingUtilNG::drawRectangle(winWidth - boxW, 0, boxW, boxH, true);
		DrawingUtilNG::drawRectangle(0, winHeight - boxH, boxW, boxH, true);

		// label "EDIT"
		glColor3ub(0, 0, 0); // black
		glRasterPos2i(winWidth - boxW + 3, boxH - 3);  // top right
		YsGlDrawFontBitmap8x12("EDIT");
		glRasterPos2i(3, winHeight - 3);  // bottom left
		YsGlDrawFontBitmap8x12("EDIT");

		// little box for instructions on adding and deleting points
		glColor3ub(255, 255, 0); // yellow
		DrawingUtilNG::drawRectangle(winWidth, winHeight, -4 * boxW, -2 * boxH, true);
		glColor3ub(0, 0, 0); // black
		glRasterPos2i(winWidth - 3.7 * boxW + 3, winHeight - boxH - 3);
		YsGlDrawFontBitmap8x12("A : Add point");
		glRasterPos2i(winWidth - 3.7 * boxW + 3, winHeight - 3);
		YsGlDrawFontBitmap8x12("D : Delete point");

	}
	else {
		// awful edit indicator (but simple to code :-)
		glColor3ub(0, 0, 0); // black
		glRasterPos2i(0, 30);  // top right
		YsGlDrawFontBitmap20x28("YOU ARE NOW IN EDIT MODE");
	}
}


void UIManager::addMainButtons(GraphicFont* aFont, int xLoc, int wid)
{
	int hei = 40;
	int spacing = 10;

	int currY = 10;
	int currX = xLoc;

	mainButtons.add(xLoc, currY, wid, hei, FSKEY_ALT, "Undo", aFont);
	
	currX += 20 + wid;
	mainButtons.add(currX, currY, wid, hei, FSKEY_SPACE, "+ Cam", aFont);

	currX = winWidth - wid - 20;
	mainButtons.add(currX, currY, wid, hei, FSKEY_M, "Advance", aFont);

}

void UIManager::addAdvanceButtons(GraphicFont* aFont, int xLoc, int yLoc)
{
	int hei = 40;
	int wid = 180;
	int spacing = 10;

	int currY = yLoc;
	int currX = xLoc;

	currX += 10;
	currY += 10;
	advanceMenuButtons.add(currX, currY, wid, hei, FSKEY_L, "Load Point Cloud", aFont);

	currX += 10 + wid;
	advanceMenuButtons.add(currX, currY, wid, hei, FSKEY_P, "Preview", aFont);

	currX += 10 + wid;
	advanceMenuButtons.add(currX + 40, currY, hei / 2, hei / 2, FSKEY_M, " X ", aFont);

	currX = currX - 2 * wid - 20;
	currY += 10 + hei;
	advanceMenuButtons.add(currX, currY, wid, hei, FSKEY_R, "Set Identitiy", aFont);

	currX += 10 + wid;
	advanceMenuButtons.add(currX, currY, wid, hei, FSKEY_O, "Export Video", aFont);

	currX = currX - wid - 10;
	currY += 10 + hei;
	advanceMenuButtons.add(currX, currY, wid, hei, FSKEY_C, "Pseudo Color", aFont);

	currX += 10 + wid;
	advanceMenuButtons.add(currX, currY, wid, hei, FSKEY_H, "Help", aFont, "See terminal for more help information");

}

void UIManager::sendUserToConsole() {
	glColor3f(0, 0, 0);
	glRasterPos2d(100, 200);
	YsGlDrawFontBitmap20x28("Input required on console . . .");
	FsSwapBuffers();
}

bool UIManager::loadPointCloudFile()
{
	fileName = DrawingUtilNG::getStringFromScreen("Enter name of file to load.",
		"Press ENTER when done, ESC to cancel.");
	if (fileName.length() > 0) {

		if (fileName.find(".ply") == string::npos)
			fileName += ".ply";

		inFile.open(fileName);
		if (inFile.is_open()) {
			data3D.init(fileName);
			inFile.close();
			cout << "       A new point cloud was loaded from file " << fileName << endl;

			// change zoom to show newly added shape
			const int percentPrint = 5;
			int step = data3D.thePoint.size / (100 / percentPrint);
			int nextPrint = step;

			pre.convert3Dpoint(data3D);
			pre.getallvalue();
			pre.PointDownsize(0, 4);

			for (int i = 0; i < data3D.thePoint.size; i++) {
				if (i >= nextPrint)
				{
					double percent = (100 * i) / data3D.thePoint.size;
					std::cout << "\r" << "paint PLY progress: " << std::string(percent / percentPrint, '|') << percent << "%";
					std::cout.flush();
					nextPrint += step;
				}
			}
		}
		else {
			cout << "       ERROR: Was not able to open file " << fileName << endl;
			return false;
		}

	}

	return true;
}

void UIManager::drawAdvanceMeau()
{
	glColor3ub(255, 255, 255);
	DrawingUtilNG::drawRectangle((winWidth - 120) / 2 - 40, winHeight / 2, 500, 200, true);
	glColor3ub(0, 0, 0);
	DrawingUtilNG::drawRectangle((winWidth - 120) / 2 - 40, winHeight / 2, 500, 200, false);
	glColor3ub(255, 0, 0);
	DrawingUtilNG::drawRectangle((winWidth - 120) / 2 - 40 + 430, winHeight / 2 + 10, 20, 20, true);

	advanceMenuButtons.paint();

}

bool UIManager::manage() {
	int mouseEvent = 0, leftButton = 0, middleButton = 0, rightButton = 0,
		screenX = 0, screenY = 0;

	bool terminate = false;
	Point3D thePoint;


	CameraUserController cameraController;

	//initialize special fonts (after FsOpenWindow)
	ComicSansFont comicsans;
	stringstream coordStream;     // for displaying coordinates on screen

	//traj.setVelocity(10);
	bool isDisPlayMode = false;
	static int displayedFrameCnt = 0;
	vector<Campos> keyFrames;
	Campos currCamPos;
	Point currCamPoint;
	int buttonKey;

	showMenu();
	while (!terminate)
	{

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// reset all transformations
		glLoadIdentity();

		FsPollDevice();
		
		int wid, hei;
		FsGetWindowSize(wid, hei);

		// get mouse event
		mouseEvent = FsGetMouseEvent(leftButton, middleButton, rightButton, screenX, screenY);
		key = FsInkey();
		//cout << "key " << key << "mouseEvent " << mouseEvent << " " << FSKEY_L << endl;
		// check if a button was clicked
		if (key == FSKEY_NULL && mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
			buttonKey = mainButtons.checkClick(screenX, screenY);

			if (buttonKey != FSKEY_NULL)
				key = buttonKey;  // pretend the user pressed a key 
		}

		if (showAdvanceMenu == true && key == FSKEY_NULL && mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
			buttonKey = advanceMenuButtons.checkClick(screenX, screenY);

			if (buttonKey != FSKEY_NULL)
				key = buttonKey;  // pretend the user pressed a key 
		}

		switch (key)
		{
		case FSKEY_ESC:
			terminate = true;
			break;
		case FSKEY_L:
			loadPointCloudFile();
			showMenu(); // So that it is "fresh"
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
			showAdvanceMenu = false;
			cameraController.camera.getCameraKeyFrames(keyFrames);
			cout << " isDisPlayMode " << isDisPlayMode;
			break;
		case FSKEY_M:
			showAdvanceMenu = !showAdvanceMenu;
			break;
		case FSKEY_H:
			showMenu();
			break;
		case FSKEY_C:
			break;
		case FSKEY_O:
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

		pre.drawPoint(data3D);
		cameraController.drawCameraKeyFrame();
		drawGridAndAxis();

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		comicsans.setColorHSV(0, 1, 1);
		/*comicsans.drawText("Testing Camera control APIs!", 10, 60, .25);*/
		std::string data;
		comicsans.drawText(cameraController.getCurrCameraParameterString(), 10, 150, .15);

		mainButtons.paint();
		//mainButtons.checkHover(screenX, screenY); // remove hover feedback for better performance ?
		if (showAdvanceMenu) {
			drawAdvanceMeau();
			//advanceMenuButtons.checkHover(screenX, screenY);
		}


		//display coords of mouse
		if (leftButton && mouseEvent == FSMOUSEEVENT_MOVE) { // write coords on screen if left button is held down
			coordStream.str("");  // reset stream
			coordStream.precision(4);
			coordStream << " (" << screenX << ", " << screenY << ")";
			comicsans.setColorHSV(0, 1, 1);
			comicsans.drawText(coordStream.str().c_str(), screenX, screenY - 3, .10);
			cameraController.onMouseClick(screenX, screenY, 0);
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
			//cout << cameraController.camera.posToString(frame) << endl;

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

	return true;
}

