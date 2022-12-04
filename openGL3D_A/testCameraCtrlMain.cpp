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
#include "UIManager.h"


int main(void)
{
	// set up UI manager
	UIManager theManager(900, 600);
	theManager.manage();
	//while (theManager.manage()) {
	//	// actually display graphics
	//	FsSwapBuffers();

	//	// prepare for next loop
	//	FsSleep(25);
	//}

	return 0;
}