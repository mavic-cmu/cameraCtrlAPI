#pragma once
/*****************************************************************//**
 * \file   CameraUIInterface.h
 * \brief
 *
 * \author chenw
 * \date   November 2022
 *********************************************************************/
#include <iostream>
#include "Camera3D.h"

class CameraUserController {
private:
	int preMousePosX = 0, preMousePosY = 0;

public:
	Camera3D camera;
	/**
	 * function to handle keyboard input. e.g. control camera postition and save key frame etc.
	 *
	 * \param direction
	 */
	void onKeyBoardPress(CamMoveDirct direction);

	/**
	 * Function to handle mouse event. e.g. control rotation of camera position.
	 *
	 * \param x the x coordinate of screen
	 * \param y the y coordinate of screen
	 */
	void onMouseClick(int x, int y);

	/**
	 * Draw 3D camera boxes of each key frames.
	 *
	 */
	void drawCameraKeyFrame(void);

	/**
	 * Record current camera poistion ad key frame.
	 *
	 */
	void addCameraKeyFrame(void);

	/**
	 * Delete the last one in key frames.
	 *
	 */
	void deleteCameraKeyFrame(void);

	/**
	 * Reset camera to initial position.
	 *
	 */
	void resetCameraInitPos(void);

	/**
	 * Set camera position to target. This function is for trajectory display.
	 *
	 * \param pos
	 */
	void setCameraToPos(Campos pos);

	/**
	 * Convert current camera position to string, debug purpose.
	 *
	 * \return
	 */
	std::string getCurrCameraParameterString(void);

};
