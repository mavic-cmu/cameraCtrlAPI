#include "trajGenerator.h"

int main() {
	// create a trajGenerator object
	trajGenerator trajGen;

	// TEST CASE 1
	std::cout << "<<<<<<<<<<<< TEST 1 <<<<<<<<<<<<" << std::endl;

	std::cout << std::endl << "Generate the default trajectory" << std::endl;
	trajGen.resetTraj(100.0, 0.0);
	trajGen.printTraj();
	std::cout << std::endl;

	// TEST CASE 2
	std::cout << "<<<<<<<<<<<< TEST 2 <<<<<<<<<<<<" << std::endl;

	std::cout << std::endl << "Add two points:" << std::endl;
	Point keyPoint_1 = { {100.0, 0.0, 0.0}, {1.0, 0.0, 0.0, 0.0} };  // key point 1
	Point keyPoint_2 = { {0.0, 100.0, 0.0}, {1.0, 0.0, 0.0, 0.0} };  // key point 2
	trajGen.addKeyPoint(keyPoint_1);
	trajGen.addKeyPoint(keyPoint_2);
	trajGen.printKeyPoints();

	std::cout << std::endl << "Insert a point between them:" << std::endl;
	Point keyPoint_3 = { {50.0, 50.0, 0.0}, {1.0, 0.0, 0.0, 0.0} };  // key point 3
	trajGen.insertKeyPoint(keyPoint_3, 1);
	trajGen.printKeyPoints();

	std::cout << std::endl << "Add another point:" << std::endl;
	Point keyPoint_4 = { {0.0, 200.0, 0.0}, {0.7071068, 0.0, 0.0, 0.7071068} };
	trajGen.addKeyPoint(keyPoint_4);
	trajGen.printKeyPoints();

	std::cout << std::endl << "Remove the point with index 2:" << std::endl;
	trajGen.removeKeyPoint(2);
	trajGen.printKeyPoints();

	std::cout << std::endl << "Generate a trajectory:" << std::endl;
	trajGen.genTraj(1);
	trajGen.printTraj();

	std::cout << std::endl << "Change camera speed and regenerate the trajectory" << std::endl;
	trajGen.setVelocity(10.0);
	trajGen.genTraj(1);
	trajGen.printTraj();

	std::cout << std::endl;

	return 0;
}