#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctype.h>
#include <vector>
#include "fssimplewindow.h"
#include "GraphicFont.h"

void developFontData(); // used only when creating a new font

int main(void)
{
	using namespace std;
	using namespace DrawingUtilNG;

	if (false) {               // set to true to manually get font coordinates
		developFontData();
		return 0;             //  <<<<<<<<<<< possible exit from function
	}

	// must "open window" before declaring fonts
	FsOpenWindow(16, 16, 1100, 800, 1);

	int currAngle = 0;
	int currX = 10;
	int key = FSKEY_NULL;
	ImpactFont impact;
	GaramondFont garamond;
	JokermanFont jokerman;
	ComicSansFont comicsans;

	while (FSKEY_ESC != key)
	{
		FsPollDevice();
		key = FsInkey();
		currAngle += (currAngle >= 360) ? 1 : -359;  // I don't normally do this
													 // but thought you'd like an example

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// draw black rectangle
		glColor3d(0.0, 0.0, 0.0);
		drawRectangle(0, 0, 800, 600, true);

		impact.setColorHSV(20, 1, 1); // reddish orange
		impact.drawText("Test01: CDEFG where I try it!", 30, 300, .5, 11);

		//// note mirrored text and how rotation changes meaning
		//impact.setColorHSV(280, 1, 1, .5); // purple translucent
		//impact.drawText("TEST 2 ON OF QO FOR", 150, 18, -2, 90);

		//garamond.setColorHSV(120, 1, 1); // green and spinning
		//garamond.drawText("Test03: where I try it!", 730, 300, .5, currAngle);

		//garamond.setColorHSV(0, 1, 1, 0.7); // red translucent
		//garamond.drawText("TEST 4 ON OF QO FOR", 840, 800, 1, 90);

		jokerman.setColorHSV(90, 1, 1, 1); //  
		//jokerman.drawText("A sample EQUATION: 56+37=3.45*X, but I'm not sure that actually works."
		//	, currX, 300, 1, 0);
		jokerman.drawText("AaBcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
			, currX, 300, 1, 0);
		jokerman.setColorHSV(190, 1, 1, 1); //  
		jokerman.drawText("Use left and right arrow to move large text above", 10, 500, .3, 0);

		string longText = "You spin me right round, baby, right round; like a record, baby, right round, round, round. I want your love, looooooove. I need your love, looooove. All I know is that to me, you look like you're lots of fun. Open up your loving arms 'cause here I come. You spin me.";
		comicsans.setColorHSV(270, 1, 1, 1);
		comicsans.drawTextCircle(longText, 500, 300, 200, .25, currAngle, 1);

		impact.setColorHSV(20, 1, 1); // reddish orange
		impact.drawText("Test01: CDEFG where I try it!", 30, 300, .5, 11);

		switch (key) {
		case FSKEY_LEFT: currX -= 30;
			break;
		case FSKEY_RIGHT: currX += 30;
			break;
		}
		FsSwapBuffers();
		FsSleep(30);

	}
}


void developFontData()
{
	using namespace std;
	using namespace DrawingUtilNG;
	string testFont = "ComicSansFont01.png";   // change to font image you are developing

	// instructions
	cout << "Use the mouse to locate the line that indicates the location of the start of the font" << endl;
	cout << "Use arrow keys for slight adjustment." << endl;
	cout << "Press space bar to output coordinates of bottom of test line." << endl;
	cout << "Go in order to get an output for left and output for right of each letter." << endl;
	cout << "When you are done, select all the coords " << endl;
	cout << "(using left button on terminal) and copy(using ENTER key on terminal" << endl;
	cout << "Paste into Excel and create the txt file for the font." << endl << endl;


	int lb, mb, rb, x, y;
	int currX = 0, currY = 0;

	YsRawPngDecoder fontImage;
	fontImage.Decode(testFont.c_str());

	FsOpenWindow(16, 16, fontImage.wid, fontImage.hei, 1);

	GLuint texId;  // this will be used as texture identifiers for the two PNGs

	// glGenTextures(2,texId);  // You can also reserve two texture identifies with one call this way.

	glGenTextures(1, &texId);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, texId);  // Making the texture identifier current (or bring it to the deck)

	// set up parameters for the current texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,    // Level of detail
		GL_RGBA,       // the "A" in RGBA will include the transparency
		fontImage.wid,    // the hippos width and height
		fontImage.hei,
		0,    // Border width, but not supported and needs to be 0.
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		fontImage.rgba);

	int key = FSKEY_NULL;
	while (FSKEY_ESC != key)
	{
		FsPollDevice();

		key = FsInkey();

		int wid, hei;
		FsGetWindowSize(wid, hei);   // nice little function

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		// draw black rectangle
		glColor3d(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);

		glVertex2i(0, 0);
		glVertex2i(wid, 0);
		glVertex2i(wid, hei);
		glVertex2i(0, hei);
		glEnd();

		// in order for textures to show, you must create a projection
		glViewport(0, 0, wid, hei);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor4d(1.0, 0.0, 0.0, 1.0);   // this color will "tint" the image

		// enable texture mapping
		glEnable(GL_TEXTURE_2D);

		// turn on blending so that transparency can be applied
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// bind a texture to OpenGL primitives
		glBindTexture(GL_TEXTURE_2D, texId);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);   // these are "percentage" of the image
		glVertex2i(0, 0);        // these are actual pixel coordinates on screen
		glTexCoord2d(1.0, 0.0);
		glVertex2i(wid, 0);
		glTexCoord2d(1.0, 1.0);
		glVertex2i(wid, hei);
		glTexCoord2d(0.0, 1.0);
		glVertex2i(0, hei);

		glEnd();

		// turn off blending 
		glDisable(GL_BLEND);
		// turn off texture 
		glDisable(GL_TEXTURE_2D);

		int mouse = FsGetMouseEvent(lb, mb, rb, x, y);


		if (mouse == FSMOUSEEVENT_LBUTTONDOWN) {
			currX = x; currY = y;
		}

		glColor3d(0.0, 1.0, 1.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(currX, currY);
		glVertex2i(currX, currY - 80);
		glEnd();

		switch (key) {
		case FSKEY_SPACE: cout << currX << " " << currY << endl;
			break;
		case FSKEY_LEFT: currX--;
			break;
		case FSKEY_RIGHT: currX++;
			break;
		case FSKEY_UP: currY--;
			break;
		case FSKEY_DOWN: currY++;
			break;
		}

		FsSwapBuffers();
		FsSleep(25);
	}

}