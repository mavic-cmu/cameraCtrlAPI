#include <stdio.h>
#include <math.h>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Camera3D.h"
#include "GraphicFont.h"

int main(void)
{
	bool terminate = false;
	Camera3D camera;

	FsOpenWindow(16, 16, 800, 600, 1);

	//initialize special fonts (after FsOpenWindow)
	ComicSansFont comicsans;
	comicsans.setColorHSV(0, 1, 1);

	while (!terminate)
	{
		FsPollDevice();

		int wid, hei;
		FsGetWindowSize(wid, hei);

		int key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = true;
			break;
		}

		if (FsGetKeyState(FSKEY_LEFT))
			camera.changeCameraRotation(MOVE_YAW, 1.0);

		if (FsGetKeyState(FSKEY_RIGHT))
			camera.changeCameraRotation(MOVE_YAW, -1.0);

		if (FsGetKeyState(FSKEY_UP))
			camera.changeCameraRotation(MOVE_PITCH, 1.0);

		if (FsGetKeyState(FSKEY_DOWN))
			camera.changeCameraRotation(MOVE_PITCH, -1.0);

		if (FsGetKeyState(FSKEY_W)) {
			camera.changeCameraTransition(MOVE_FORWARD, 0);
		}
		if (FsGetKeyState(FSKEY_S)) {
			camera.changeCameraTransition(MOVE_BACKWARD, 0);
		}

		if (FsGetKeyState(FSKEY_A)) {
			camera.changeCameraTransition(MOVE_LEFT, 0);
		}
		if (FsGetKeyState(FSKEY_D)) {
			camera.changeCameraTransition(MOVE_RIGHT, 0);
		}


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, wid, hei);

		// Set up 3D drawing
		camera.setUpCameraProjection();
		camera.setUpCameraTransformation();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);

		// 3D drawing from here
		glColor3ub(0, 0, 255);

		// draw floor
		glBegin(GL_LINES);
		int x;
		for (x = -500; x <= 500; x += 20)
		{
			glVertex3i(x, 0, -500);
			glVertex3i(x, 0, 500);
			glVertex3i(-500, 0, x);
			glVertex3i(500, 0, x);
		}
		glEnd();

		// draw some boxes/buildings
		glColor3ub(120, 120, 120);
		DrawingUtilNG::drawCube({ 110, 0, 120 }, { 140, 10, 140 });
		glColor3ub(120, 255, 120);
		DrawingUtilNG::drawCube({ 80, 0, -70 }, { 90, 15, -30 }, true);

		//glColor3ub(120, 255, 120);
		//DrawingUtilNG::drawSphere({ 50, 0, 50 }, 10);

		// draw axes (x is red, y is green, z is blue, like in all drawing software)
		glLineWidth(8);
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


		FsSwapBuffers();
		FsSleep(10);
	}

	return 0;

}