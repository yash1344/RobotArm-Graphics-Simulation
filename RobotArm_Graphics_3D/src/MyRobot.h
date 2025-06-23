/*
 * MyRobot.h
 *
 *  Created on: Apr 10, 2025
 *      Author: vagha
 */

#ifndef MYROBOT_H_
#define MYROBOT_H_
#include <vector>

#include "RobotParts/RobotPart.h"
#include <functional>

using std::vector;

class MyRobot {
private:
	vector<RobotPart*> robotParts;
	Coordinate target_point;
	std::function<void(void)> onChange;

	void perform_inverse_kinematics();

public:
	MyRobot();
	virtual ~MyRobot();
	RobotPart* addRobotPart(RobotPart *part);
	RobotPart* getRobotPart_index(int index);
	vector<RobotPart*>& getRobotParts();

	void achieve_TCP(Coordinate target);
	void setOnChange(const std::function<void(void)> &onChange);
	const Coordinate& getTargetPoint() const;
	void setTargetPoint(const Coordinate &targetPoint);

	friend std::ostream& operator<<(std::ostream &os, MyRobot &obj);
};

#endif /* MYROBOT_H_ */
