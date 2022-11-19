#pragma once
#include <vector>

typedef struct {
	double x;
	double y;
	double z;
	double h;
	double p;
	double b;
} Campos;

enum CamMoveDirct {
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_ROLL,
	MOVE_PITCH,
	MOVE_YAW
};

class Camera3D
{
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

