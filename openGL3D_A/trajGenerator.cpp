#include "trajGenerator.h"

std::vector<Point> trajGenerator::linearInterpolate(Point p1, Point p2) {
	std::vector<Point> result;

	double distance = sqrt(pow(p1.pos.x - p2.pos.x, 2) + pow(p1.pos.y - p2.pos.y, 2) + pow(p1.pos.z - p2.pos.z, 2));
	int steps = distance / velocity;
	for (int i = 0; i < steps; i++) {
		Point aPoint;
		aPoint.pos.x = p1.pos.x + (p2.pos.x - p1.pos.x) * i / steps;
		aPoint.pos.y = p1.pos.y + (p2.pos.y - p1.pos.y) * i / steps;
		aPoint.pos.z = p1.pos.z + (p2.pos.z - p1.pos.z) * i / steps;

		// aPoint.quat = Utils::slerp(p1.quat, p2.quat, double(i) / steps);

		std::vector<double> Euler = Utils::quat2Euler(
			Utils::slerp(
				Utils::euler2Quat(p1.roll, p1.pitch, p1.yaw),
				Utils::euler2Quat(p2.roll, p2.pitch, p2.yaw),
				double(i) / steps)
		);

		aPoint.roll = Euler[0];
		aPoint.pitch = Euler[1];
		aPoint.yaw = Euler[2];

		result.push_back(aPoint);
	}
	return result;
}

void trajGenerator::genTraj(int type) {
	traj.clear();
	if (keyPoints.size() > 1) {
	// type = 1: linear interpolation between each pair of neighboring key points
		if (type == 1) {
			for (int i = 0; i < keyPoints.size() - 1; i++) {
				std::vector<Point> interpolatedPoints = linearInterpolate(keyPoints[i], keyPoints[i + 1]);
				if (interpolatedPoints.size() > 0) {
					interpolatedPoints.pop_back();
					traj.insert(traj.end(), interpolatedPoints.begin(), interpolatedPoints.end());
				}
			}
		}

		traj.push_back(keyPoints[keyPoints.size() - 1]);
	}
}

void trajGenerator::resetTraj(double radius, double height) {
	traj.clear();
	// the default trajectory is a circle around the Z axis
	double omgea = velocity / radius;  // angular velocity (rad per frame)
	double steps = 2 * M_PI / omgea;
	for (double i = 0; i < steps; i++) {
		Point aPoint;
		aPoint.pos.x = radius * cos(omgea * i);
		aPoint.pos.y = radius * sin(omgea * i);
		aPoint.pos.z = height;

		// aPoint.quat.a = cos(omgea * i / 2);
		// aPoint.quat.b = 0;
		// aPoint.quat.c = 0;
		// aPoint.quat.d = sin(omgea * i / 2);

		Quaternion quat;
		quat.a = cos(omgea * i / 2);
		quat.b = 0;
		quat.c = sin(omgea * i / 2);
		quat.d = 0;

		std::vector<double> Euler = Utils::quat2Euler(quat);
		aPoint.roll = Euler[0];
		aPoint.pitch = Euler[1];
		aPoint.yaw = Euler[2];

		traj.push_back(aPoint);
	}
}