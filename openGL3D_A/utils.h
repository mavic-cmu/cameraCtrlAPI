#pragma once

#include <cmath>
#include <vector>

struct Position {
	double x;
	double y;
	double z;
};

// a + b*i + c*j + d*k
struct Quaternion {
	double a;
	double b;
	double c;
	double d;
};

struct Point {
	Position pos;
	//Quaternion quat;
	double roll;
	double pitch;
	double yaw;
};

class Utils {
public:
	/*
	 * @brief: Spherical Linear Interpolation (SLERP)
	 * @param: q1, q2: two quaternions
	 * @param: t: interpolation parameter
	 * @return: the interpolated quaternion
	 */
	static Quaternion slerp(Quaternion q1, Quaternion q2, double t);

	/*
	 * @brief: Convert a quaternion to Euler angles
	 * @param: q: a quaternion
	 * @return: a vector of Euler angles
	 * @note: the order of the Euler angles is ZYX
	 */
	static std::vector<double> quat2Euler(Quaternion q);

	/*
	 * @brief: Convert Euler angles to a quaternion
	 * @param: roll, pitch, yaw: Euler angles
	 * @return: a quaternion
 	 * @note: the order of the Euler angles is ZYX
	 */
	static Quaternion euler2Quat(double roll, double pitch, double yaw);
};