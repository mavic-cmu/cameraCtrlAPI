#pragma once
/*****************************************************************//**
 * \file   Camera3D.h
 * \brief camera coordinate
 *							Front - Back :  X axis
 *                          Left  - Right:  Z axis
 *                          Up    - Down :  Y axis
 *							Roll         :  rotate along X axis
 *							Pitch        :  rotate along Z axis
 *							Yaw          :  rotate along Y axis
 * \author chenw
 * \date   November 2022
 *********************************************************************/
#include <string>
#include <vector>
#include "fssimplewindow.h"


typedef struct {
	float x;
	float y;
	float z;
	float yaw;
	float pitch;
	float roll;
	float nearZ; // nearest viewpoint
	float farZ;
} Campos;

enum CamMoveDirct {
	NO_MOVE,
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_ROLL,
	MOVE_PITCH,
	MOVE_YAW,
	ZOOM_IN,
	ZOOM_OUT
};

class Camera3D
{
private:
	/**
	 * draw 3D camera boxes for each keyframes positions.
	 */
	void drawCamera(Campos camera);
	GLuint cameraTextureID;

public:
	static const double PI;
	double fov, nearZ, farZ;

	Campos initialCameraPos;
	Campos currCameraPos;
	std::vector<Campos> keyFrames;

	Camera3D();
	void initialize();

	/**
	 * Set camera projection based on current camera pos, need to be called each UI frame.
	 */
	void setUpCameraProjection();

	/**
	 * Setup camera transformation, need to be called each UI frame.
	 */
	void setUpCameraTransformation();

	/**
	 * calculate forward movement vector vx vy vz.
	 */
	void getForwardVector(double& vx, double& vy, double& vz);

	/**
	 * calculate horizion movement vector.
	 */
	void getHorizonMoveVector(double& vx, double& vy, double& vz);

	/**
	 * Set initial camera pos by user.
	 */
	void setInitCamraPos(double x, double y, double z, double roll, double pitch, double yaw);

	/**
	 * Reset camera pos to initial position.
	 */
	void resetCameraPos(void);

	/**
	 * Reset camera pos to initial position, user input maxBound.
	 */
	void resetCameraPos(double xMin, double yMin, double zMin, double xMax, double yMax, double zMax);

	/**
	 * Set camera position to position, this func is for play trajectory.
	 */
	int setCameraPosition(Campos position);  // set camera position in display mode

	/**
	 * Save current position as keyframe.
	 */
	int saveCurrentKeyFrame();

	/**
	 * Delete the last position is keyframes vector.
	 */
	int deleteKeyFrame();    // only delete the last keyFrame

	/**
	 * Return current camera key frames.
	 */
	int getCameraKeyFrames(std::vector<Campos>& keyFrames);

	/**
	 * Convert postion to string for debug purpose.
	 */
	std::string posToString(Campos camera);

	/**
	 * draw a 3D camera block in the 3D world base on current camera position
	 *
	 * \return
	 */
	int   drawKeyFrameCamPos(bool drawLine);

};

