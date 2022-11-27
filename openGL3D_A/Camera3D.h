#pragma once
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
	MOVE_ROLL,
	MOVE_PITCH,
	MOVE_YAW,
	ZOOM_IN,
	ZOOM_OUT
};

class Camera3D
{
private:

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
	void setUpCameraProjection();
	void setUpCameraTransformation();

	void getForwardVector(double& vx, double& vy, double& vz);
	void getHorizonMoveVector(double& vx, double& vy, double& vz);

	void setInitCamraPos(double x, double y, double z, double roll, double pitch, double yaw);
	void resetCameraPos(void);
	void resetCameraPos(double maxBound); // calculate camera position base on maxBound
	int setCameraPosition(Campos position);  // set camera position in display mode
	void changeCameraTransition(CamMoveDirct direction, float speed);
	void changeCameraRotation(CamMoveDirct direction, float speed);
	int saveCurrentKeyFrame();
	int deleteKeyFrame();    // only delete the last keyFrame
	int getCameraKeyFrames(std::vector<Campos>& keyFrames);

	/**
	 * draw a 3D camera block in the 3D world base on current camera position
	 *
	 * \return
	 */
	int   drawKeyFrameCamPos();

};

