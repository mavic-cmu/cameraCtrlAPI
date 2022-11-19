#pragma once


class Camera3D
{
public:
	static const double PI;
	double x, y, z;  // camera location
	double h, p, b;  // camera orientation (heading, pitch, bank)

	double fov, nearZ, farZ;

	Camera3D();
	void initialize();
	void setUpCameraProjection();
	void setUpCameraTransformation();

	void getForwardVector(double& vx, double& vy, double& vz);
	// returns unit vector components of camera orientation
};

