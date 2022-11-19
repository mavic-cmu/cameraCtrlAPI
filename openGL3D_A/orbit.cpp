#include <stdio.h>
#include <math.h>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Camera3D.h"
#include "GraphicFont.h"
#include "OrbitingViewer.h"
#include "DrawingUtilNG.h"

using namespace std;

int main(void)
{
	bool terminate = false;
	Camera3D camera;
	OrbitingViewer orbit;

	camera.z = 10.0; // look at point 0,0,10

	FsOpenWindow(16, 16, 1200, 800, 1);


	//initialize fonts (after FsOpenWindow)
	ComicSansFont comicsans;
	comicsans.setColorHSV(300, 1, 1);
	ImpactFont impact;

	while (0 == terminate)
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
			orbit.h += Camera3D::PI / 180.0;

		if (FsGetKeyState(FSKEY_RIGHT))
			orbit.h -= Camera3D::PI / 180.0;

		if (FsGetKeyState(FSKEY_UP))
			orbit.p += Camera3D::PI / 180.0;

		if (FsGetKeyState(FSKEY_DOWN))
			orbit.p -= Camera3D::PI / 180.0;

		if (FsGetKeyState(FSKEY_F) && orbit.dist > 0.5)
			orbit.dist /= 1.05;

		if (FsGetKeyState(FSKEY_B) && orbit.dist < camera.farZ * .8)
			orbit.dist *= 1.05;

		orbit.setUpCamera(camera);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, wid, hei);

		// Set up 3D drawing
		camera.setUpCameraProjection();
		camera.setUpCameraTransformation();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);

		// 3D drawing from here
		glColor3ub(93, 290, 112);
		DrawingUtilNG::drawCube(-10.0, -10.0, -10.0, 10.0, 10.0, 10.0);
		//DrawingUtilNG::drawCube(0, 0, 0, 20.0, 20.0, 20.0, true, &impact);

		//sphere test
		//DrawingUtilNG::drawSphere(0, 0, 0, 20.);
		
		// draw axes
		int length = 30;
		glLineWidth(6);

		glColor3ub(255, 0, 0);  // X (red)
		glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(length, 0, 0);
		glEnd();

		glColor3ub(0, 255, 0);  // Y (green)
		glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, length, 0);
		glEnd();

		glColor3ub(0, 0, 255);  // Z (blue)
		glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, length);
		glEnd();

		comicsans.setColorHSV(200, 1, 1);
		comicsans.drawText3D("  Hello World", { 0,-20,0 }, { 0,10,0 }, { -1,-20,-1 }, .3, 20.);

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		comicsans.drawText("I'm Orbiting!", 10, 60, .25);

		std::string data;
		data = "X=" + std::to_string(camera.x) + " Y=" + std::to_string(camera.y) + " Z=" + std::to_string(camera.z);
		comicsans.setColorHSV(300, 1, .5);
		comicsans.drawText(data, 10, 80, .15);

		data = "Camera Orientation: h=" + std::to_string(camera.h * 45. / atan(1.))
			+ " deg, p=" + std::to_string(camera.p * 45. / atan(1.)) + " deg";
		comicsans.drawText(data, 10, 95, .15);

		FsSwapBuffers();
		FsSleep(10);
	}

	return 0;

}