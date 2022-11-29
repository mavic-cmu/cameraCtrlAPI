#include "utils.h"

bool operator==(const Position& lhs, const Position& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator==(const Quaternion& lhs, const Quaternion& rhs) {
	return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c && lhs.d == rhs.d;
}

bool operator==(const Point& lhs, const Point& rhs) {
	return lhs.pos == rhs.pos && lhs.roll == rhs.roll && lhs.pitch == rhs.pitch && lhs.yaw == rhs.yaw;
}

// https://splines.readthedocs.io/en/latest/rotation/slerp.html
Quaternion Utils::slerp(Quaternion q1, Quaternion q2, double t) {

	if (q1 == q2) {
		return q1;
	}
	else {
		Quaternion result;
		double dot = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
		double theta = acos(dot);
		double sinTheta = sin(theta);
		double a = sin((1 - t) * theta) / sinTheta;
		double b = sin(t * theta) / sinTheta;
		result.a = a * q1.a + b * q2.a;
		result.b = a * q1.b + b * q2.b;
		result.c = a * q1.c + b * q2.c;
		result.d = a * q1.d + b * q2.d;
		return result;
	}
}

std::vector<double> Utils::quat2Euler(Quaternion q) {
	std::vector<double> result;
	double roll = atan2(2 * (q.a * q.b + q.c * q.d), 1 - 2 * (q.b * q.b + q.c * q.c));
	double pitch = asin(2 * (q.a * q.c - q.d * q.b));
	double yaw = atan2(2 * (q.a * q.d + q.b * q.c), 1 - 2 * (q.c * q.c + q.d * q.d));
	result.push_back(roll);
	result.push_back(pitch);
	result.push_back(yaw);
	return result;
}

Quaternion Utils::euler2Quat(double roll, double pitch, double yaw) {
	Quaternion result;
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	result.a = cy * cr * cp + sy * sr * sp;
	result.b = cy * sr * cp - sy * cr * sp;
	result.c = cy * cr * sp + sy * sr * cp;
	result.d = sy * cr * cp - cy * sr * sp;
	return result;
}