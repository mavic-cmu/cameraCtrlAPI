#pragma once
/*
Nestor Gomez
Carnegie Mellon University
Eng. Computation, 24-780-B
Prob Set 9
Due Tues. Nov. 22, 2022
*/

#include <vector>
#include <fstream>

#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ButtonCollection.h"
#include "GraphicFont.h"
#include "Camera3D.h"
#include "CameraUIInterface.h"
#include "preProcess.h"
#include "trajGenerator.h"

class UIManager {
private:
	bool simulationIsRunning = false;

	int winWidth = 1000, winHeight = 600;
	int panX = 0, panY = winHeight;
	float scale = 6.f;

	std::string fileName;
	std::ifstream inFile;
	char userChoice = 'N';
	int key = FSKEY_NULL; // just to start it up

	bool inEditMode = false;
	bool showThePoints = false;
	bool useFillMode = false;

	// for moving a vertex with the mouse
	int currVertex = -1;
	bool vertexIsMoving = false;
	int mouseEvent = 0, leftButton = 0, middleButton = 0, rightButton = 0,
		screenX = 0, screenY = 0;

	// for panning and zooming
	int prevScreenX = -1000, prevScreenY = -1000;

	// buttons
	ButtonCollection theButtons;
	
	// data
	GraphicFont* screenFont; // need to use pointer so that I can declare AFTER FsOpenWindow
	
	// point cloud
	preProcess pre;
	readPLY data3D;
	trajGenerator traj;
	// audio feedback on inside/outside check (left-over from PS06 :-)
	//YsSoundPlayer theSoundPlayer;
	//YsSoundPlayer::SoundData insideSound;
	//YsSoundPlayer::SoundData outsideSound;

public:
	UIManager() {};

	UIManager(int width, int height);

	bool manage(void);

	// displays full menu of user options on console
	// barely needed now that we have buttons
	void showMenu();

protected:

	// if manager is in edit mode, this paints some indication on the graphics
	// screen that this is the case. Otherwise does nothing
	void paintEditIndicator();

	// handle mouse input (OpenGL is still in screen coords)
	//void manageMouse();

	void drawGridAndAxis(void);
	// creates the buttons to be shown on interface
	// called by constructor
	void addButtons(GraphicFont* aFont, int xLoc, int wid);

	// manage slide boxes
	void sendUserToConsole();

	// load point cloud by entry file name ion screen
	bool loadPointCloudFile();
	//void addSlideBox();
	//void removeSlideBox();
	//void printAllBoxes();
};