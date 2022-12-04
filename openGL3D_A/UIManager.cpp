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
	GraphicFont* buttonFont = new TimesNewRomanFont;
	buttonFont->setColorRGB(0, 0, 0); // black

	addButtons(buttonFont, winWidth - 108, 100);
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


//void TrackManager::manageMouse()
//{
//	// handle mouse input (OpenGL is still in screen coords)
//	stringstream coordStream;     // for displaying coordinates on screen
//	FsPollDevice();
//	mouseEvent = FsGetMouseEvent(leftButton, middleButton,
//		rightButton, screenX, screenY);
//
//	Point2D worldPnt = getWorldCoords({ screenX * 1.f, screenY * 1.f });
//
//	if (leftButton) { // write coords on screen if left button is held down
//		coordStream.str("");  // reset stream
//		coordStream.precision(4);
//		coordStream << worldPnt.x << ", " << worldPnt.y
//			<< " (" << screenX << ", " << screenY << ")";
//		glColor3ub(60, 230, 60);
//		glRasterPos2i(screenX, screenY - 3);  // set position 3 pix above
//		YsGlDrawFontBitmap7x10(coordStream.str().c_str());
//	}
//
//	if (inEditMode) {
//		float hoverDistance = 3.f / scale; // 3 pixels
//
//		//if (mouseEvent == FSMOUSEEVENT_RBUTTONDOWN) {
//		//	// put whatever code is needed to indicate if right click 
//		//	// is inside or outside of current shape
//		//	if (theShapes.at(currShape).isContained(worldPnt)) {
//		//		theSoundPlayer.PlayOneShot(insideSound);
//		//	}
//		//	else 
//		//		theSoundPlayer.PlayOneShot(outsideSound);
//		//}
//
//		// figure out if there's a point near mouse location
//		if (!vertexIsMoving) { // avoid changing currVertex while moving a vertex
//			currVertex = theTrack.getIndex(worldPnt, hoverDistance);
//			//if (currVertex > 0)
//			//	cout << "Hovered over Pnt " << currVertex << endl;
//		}
//		if (mouseEvent == FSMOUSEEVENT_LBUTTONUP) {
//			vertexIsMoving = false;
//		}
//		else if (leftButton && mouseEvent == FSMOUSEEVENT_MOVE && currVertex > 0) {
//			vertexIsMoving = true;
//			theTrack.movePoint(worldPnt, currVertex);
//		}
//	}
//
//	// capture location of first button press (needed for panning and zooming)
//	if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN || mouseEvent == FSMOUSEEVENT_MBUTTONDOWN) {
//		prevScreenX = screenX; prevScreenY = screenY;
//	}
//
//	// disallow panning and zooming with mouse when a vertex is moving
//	if (!vertexIsMoving) {
//
//		// pan in x and y axes when Ctrl key is held down and left button is down
//		// note: I added middle button (wheel) drag for panning
//		if (middleButton || (FsGetKeyState(FSKEY_CTRL) && leftButton)) {
//			// no need for scale since the screen-to-model ratio is not applicable
//			panX += (screenX - prevScreenX);
//			panY += (screenY - prevScreenY);
//			prevScreenX = screenX; prevScreenY = screenY; // reset previous values to continue move
//		}
//
//		// zoom in and out when Shft key is held down and left button is down
//		// note: I added wheel rolling for zoomimg, which accidentally is also
//		//       triggered by touchpad pinch and two finger scroll
//		else if (key == FSKEY_WHEELUP || key == FSKEY_WHEELDOWN
//			|| (FsGetKeyState(FSKEY_SHIFT) && leftButton)) {
//			double oldScale = scale;
//			if (key == FSKEY_WHEELUP)
//				scale *= 1.03; // less jumpy than zooming with +/- keys
//			else if (key == FSKEY_WHEELDOWN)
//				scale /= 1.02;
//			else if (screenY < prevScreenY)
//				scale *= max(1.02, (prevScreenY - screenY) * 0.1);
//			else if (screenY > prevScreenY)
//				scale /= max(1.02, (prevScreenY - screenY) * -0.1);
//
//			// adjust panX and panY so point under mouse does not move
//			// i.e., we can zoom in/out on a specific point
//			// a bit complicated since you have to convert old origin to screen coords
//			// then adjust pan, then convert to model coords. 
//			// what you see below is the simplified equation after all substitutions
//			// rounding reduces "shifting"
//			if (key == FSKEY_WHEELUP || key == FSKEY_WHEELDOWN) {
//				panX = (int)round((screenX * (oldScale - scale)
//					+ panX * scale) / oldScale);
//				panY = (int)round((screenY * (oldScale - scale)
//					+ panY * scale) / oldScale);
//			}
//			prevScreenX = screenX; prevScreenY = screenY; // reset previous values to continue move
//		}
//	}
//}

void UIManager::addButtons(GraphicFont* aFont, int xLoc, int wid)
{
	int hei = 30;
	int spacing = 10;

	int currY = 30;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_L, "Load", aFont,
		"Load a track from a file");
	
	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_S, "Save", aFont,
		"Save track to a file");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_H, "Hue", aFont,
		"Cycle through several colors of track");

	//currY += hei + spacing;
	//theButtons.add(xLoc, currY, wid, hei, FSKEY_Q, "Sound", aFont,
	//	"Toggle sound feedback on/off");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_Z, "View All", aFont,
		"Adjust view to fit the whole track");

	currY += hei + spacing * 3;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_E, "Edit", aFont,
		"Toggle Edit Mode");

	currY += hei + spacing * 3;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_B, "Add Box", aFont,
		"Add a box to the track");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_R, "Remove", aFont,
		"Remove box from the track");

	currY += hei + spacing;
	theButtons.add(xLoc, currY, wid, hei, FSKEY_I, "Initial", aFont,
		"Initialize model (put all boxes at initial positions)");

	currY += hei + spacing * 3;
	GraphicFont* boldFont = new JokermanFont;
	boldFont->setColorRGB(1., 0., 0.);
	theButtons.add(xLoc, currY, wid, hei, FSKEY_SPACE, "GO/STOP", boldFont,
		"Start/Stop the simulation");

	// to disable a button (will gray out and won't return its value)
	//theButtons.disableButton(FSKEY_SPACE);

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
	//vector<Point> generatedTraj;

	//data3D.init("flowers.ply");
	//cout << endl;
	//// Example how to access the data, first element
	//std::cout << "x value of the first vertex element: " << data3D.thePoint.x->at<float>(0) << std::endl;
	//std::cout << "y value of the first vertex element: " << data3D.thePoint.y->at<float>(0) << std::endl;
	//std::cout << "z value of the first vertex element: " << data3D.thePoint.z->at<float>(0) << std::endl;
	//std::cout << "nx value of the first vertex element: " << data3D.thePoint.nx->at<float>(0) << std::endl;
	//std::cout << "ny value of the first vertex element: " << data3D.thePoint.ny->at<float>(0) << std::endl;
	//std::cout << "nz value of the first vertex element: " << data3D.thePoint.nz->at<float>(0) << std::endl;
	//std::cout << "r value of the first vertex element: " << static_cast<unsigned int>(data3D.thePoint.r->at<unsigned char>(0)) << std::endl;
	//std::cout << "g value of the first vertex element: " << static_cast<unsigned int>(data3D.thePoint.g->at<unsigned char>(0)) << std::endl;
	//std::cout << "b value of the first vertex element: " << static_cast<unsigned int>(data3D.thePoint.b->at<unsigned char>(0)) << std::endl;
	//std::cout << "alpha value of the first vertex element: " << static_cast<unsigned int>(data3D.thePoint.alpha->at<unsigned char>(0)) << std::endl;
	//std::cout << "vertex size: " << data3D.thePoint.size << std::endl;

	/*const int percentPrint = 5;
	int step = data3D.thePoint.size / (100 / percentPrint);
	int nextPrint = step;

	pre.convert3Dpoint(data3D);
	pre.getallvalue();
	pre.PointDownsize(0, 4);*/

	//for (int i = 0; i < data3D.thePoint.size; i++) {
	//	if (i >= nextPrint)
	//	{
	//		double percent = (100 * i) / data3D.thePoint.size;
	//		std::cout << "\r" << "paint PLY progress: " << std::string(percent / percentPrint, '|') << percent << "%";
	//		std::cout.flush();
	//		nextPrint += step;
	//	}
	//}

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
			buttonKey = theButtons.checkClick(screenX, screenY);

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
		/*	currCamPos = cameraController.getCurrCameraPos();
			currCamPoint = { {currCamPos.x, currCamPos.y, currCamPos.z}, currCamPos.roll / 45. * atan(1.), currCamPos.pitch / 45. * atan(1.), currCamPos.yaw / 45. * atan(1.) };
			traj.addKeyPoint(currCamPoint);
			cameraController.addCameraKeyFrame();
			cout << "Add one key frame" << endl;*/
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
		comicsans.drawText("Testing Camera control APIs!", 10, 60, .25);
		std::string data;
		comicsans.drawText(cameraController.getCurrCameraParameterString(), 10, 95, .15);

		theButtons.paint();
		theButtons.checkHover(screenX, screenY); // remove hover feedback for better performance ?


		//display coords of mouse
		if (leftButton && mouseEvent == FSMOUSEEVENT_MOVE) { // write coords on screen if left button is held down
			coordStream.str("");  // reset stream
			coordStream.precision(4);
			coordStream << " (" << screenX << ", " << screenY << ")";
			comicsans.setColorHSV(0, 1, 1);
			comicsans.drawText(coordStream.str().c_str(), screenX, screenY - 3, .10);
			cameraController.onMouseClick(screenX, screenY, 0);
		}

		////play mode
		//if (isDisPlayMode) {
		//	traj.genTraj(1);
		//	generatedTraj = traj.getTraj();
		//}

		//if (isDisPlayMode && displayedFrameCnt < generatedTraj.size()) {
		//	Campos frame = cameraController.getCurrCameraPos();
		//	frame.x = generatedTraj.at(displayedFrameCnt).pos.x;
		//	frame.y = generatedTraj.at(displayedFrameCnt).pos.y;
		//	frame.z = generatedTraj.at(displayedFrameCnt).pos.z;
		//	frame.yaw = generatedTraj.at(displayedFrameCnt).yaw / atan(1.) * 45.;
		//	frame.pitch = generatedTraj.at(displayedFrameCnt).pitch / atan(1.) * 45.;
		//	frame.roll = generatedTraj.at(displayedFrameCnt).roll / atan(1.) * 45.;
		//	cameraController.setCameraToPos(frame);
		//	cout << cameraController.camera.posToString(frame) << endl;

		//	if (displayedFrameCnt == generatedTraj.size() - 1) {
		//		isDisPlayMode = false;
		//	}
		//}
		//else if (isDisPlayMode == false || displayedFrameCnt == generatedTraj.size() - 1) {
		//	displayedFrameCnt = 0;

		//}
		FsSwapBuffers();
		FsSleep(10);
		//displayedFrameCnt++;
	}

	return true;
}


//bool UIManager::manage()
//{
//	ofstream outFile;
//	int buttonKey;
//
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//
//	// reset all transformations
//	glLoadIdentity();
//
//	FsPollDevice();
//	key = FsInkey();
//
//	// handle mouse input (OpenGL is still in screen coords)
//	//manageMouse();
//	mouseEvent = FsGetMouseEvent(leftButton, middleButton, rightButton, screenX, screenY);
//	// check if a button was clicked
//	if (key == FSKEY_NULL && mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
//		buttonKey = theButtons.checkClick(screenX, screenY);
//
//		if (buttonKey != FSKEY_NULL)
//			key = buttonKey;  // pretend the user pressed a key 
//	}
//
//	switch (key) {
//	case FSKEY_L: // load a file and add to shapes
//		fileName = DrawingUtilNG::getStringFromScreen("Enter name of file to load.",
//			"Press ENTER when done, ESC to cancel.");
//		if (fileName.length() > 0) {
//
//			if (fileName.find(".track") == string::npos)
//				fileName += ".track";
//
//			inFile.open(fileName);
//			//if (inFile.is_open()) {
//			//	theTrack.readFile(inFile);
//			//	inFile.close();
//			//	cout << "       A new track was loaded from file " << fileName << endl;
//
//			//	// change zoom to show newly added shape
//			//	resetView();
//			//}
//			//else
//			//	cout << "       ERROR: Was not able to open file " << fileName << endl;
//		}
//		showMenu(); // So that it is "fresh"
//		break;
//		//case FSKEY_RIGHT: panX += 5;
//		//	break;
//		//case FSKEY_LEFT: panX -= 5;
//		//	break;
//		//case FSKEY_UP: panY -= 5;
//		//	break;
//		//case FSKEY_DOWN: panY += 5;
//		//	break;
//		//case FSKEY_PLUS: scale *= 1.05;
//		//	break;
//		//case FSKEY_MINUS: scale /= 1.05;
//		//	break;
//
//		//	// new stuff for PS04
//		//case FSKEY_Z:   // reset Zoom to fit all shapes
//		//	resetView();
//		//	break;
//		//case FSKEY_E:
//		//	inEditMode = !inEditMode;
//		//	break;
//		//case FSKEY_S: // save current shape (similar to load shape above)
//		//	//sendUserToConsole();
//		//	//cout << "Filename for saving current track >> ";
//		//	//cin >> fileName;
//		//	fileName = DrawingUtilNG::getStringFromScreen("Enter name of file to save.",
//		//		"Press ENTER when done, ESC to cancel.");
//		//	if (fileName.length() > 0) {
//
//		//		if (fileName.find(".track") == string::npos)
//		//			fileName += ".track";
//
//		//		outFile.open(fileName);
//		//		if (outFile.is_open()) {
//		//			outFile << theTrack;
//		//			outFile.close();
//		//			cout << "Just saved " << fileName << " to current folder." << endl;
//		//		}
//		//		else {
//		//			cout << "Cannot open file " << fileName << " for outFile." << endl;
//		//		}
//		//		showMenu();
//		//	}
//		//	break;
//		//case FSKEY_H:
//		//	theTrack.changeColor(10.f);
//		//	break;
//		//case FSKEY_B:
//		//	addSlideBox();
//		//	break;
//		//case FSKEY_R:
//		//	removeSlideBox();
//		//	break;
//		//case FSKEY_I:  // set to initial state
//		//	for (auto& currBox : theBoxes) {
//		//		currBox.reset();
//		//	}
//		//	break;
//		//case FSKEY_SPACE:
//		//	simulationIsRunning = !simulationIsRunning;
//		//	break;
//
//	}
//
//	//if (inEditMode) {  // also for PS04
//	//	paintEditIndicator();
//	//	switch (key) {
//	//	case FSKEY_A:   // add a point for spline control
//	//		if (currVertex > 0) { // there is a highlighted point
//	//			theTrack.addPoint(currVertex, 0.8);
//	//		}
//	//		break;
//	//	case FSKEY_D:   // delete a point
//	//		if (currVertex > 0) { // there is a highlighted point
//	//			theTrack.removePoint(currVertex);
//	//		}
//	//		break;
//	//	case FSKEY_P:  // toggle showing points for current shape
//	//		showThePoints = !showThePoints;
//	//		break;
//	//	}
//	//}
//
//	// draw edit mode indicator
//	//if (inEditMode)
//	//	paintEditIndicator();
//
//	// set up axes to "math normal" (origin at lower left, y-axis going up)
//	// and pan and scale
//	//glTranslatef(panX, panY, 0);
//	//glScalef(scale, -scale, 1);
//
//	//theTrack.paint(false, useFillMode, showThePoints || inEditMode);
//
//	//// highlight point that could be selected
//	//if (inEditMode && currVertex > 0) {
//	//	glColor3ub(215, 215, 0);
//	//	theTrack.paintPoint(currVertex, 6. / scale);
//	//}
//
//	//int i = 2;
//	//double red, green, blue;
//	//for (auto& currBox : theBoxes) {
//	//	DrawingUtilNG::hsv2rgb(i++ * 60, 1, 1, red, green, blue);
//	//	glColor3f(red, green, blue);
//
//	//	currBox.paint();
//	//	if (simulationIsRunning)
//	//		currBox.move(.025);  // should change this to do "real" time
//	//}
//
//	// reset all transformations and paint the buttons on top of everything
//	glLoadIdentity();
//	theButtons.paint();
//	theButtons.checkHover(screenX, screenY); // remove hover feedback for better performance ?
//
//	//// paint the box data (not required for PS09)
//	//string boxData;
//	//int j = 2;
//	//for (auto& currBox : theBoxes) {
//	//	if (simulationIsRunning)
//	//		boxData = currBox.getDynamicData();
//	//	else
//	//		boxData = currBox.getStaticData();
//
//	//	screenFont->setColorHSV(j * 60, 1, 1);
//	//	screenFont->drawText(boxData, 20, j++ * 25, 20./screenFont->getLetterHeight() );
//	//}
//
//
//	return key != FSKEY_ESC;
//}