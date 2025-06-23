/*
 * MyRobot.cpp
 *
 *  Created on: Apr 10, 2025
 *      Author: vagha
 */

#include "MyRobot.h"

#include <stdexcept>
#include <iostream>
#include "Shapes/Line.h"
#include <thread>
#include <utility>

#include "Utility.h"
using namespace std;
MyRobot::MyRobot() {
}

MyRobot::~MyRobot() {
	for (RobotPart *part : robotParts) {
		delete part;
	}
	robotParts.clear();
}

RobotPart* MyRobot::addRobotPart(RobotPart *part) {
	robotParts.push_back(part);
	return part;
}

RobotPart* MyRobot::getRobotPart_index(int index) {
	if (index < (int) robotParts.size()) {
		return robotParts[index];
	}
	throw std::out_of_range("index is out of range");
}

vector<RobotPart*>& MyRobot::getRobotParts() {
	return robotParts;
}

void MyRobot::perform_inverse_kinematics() {
	thread thred1(
			[this]() {
				const float target_threshold = 1;
				int count = 0;

				auto rb_iterator = robotParts.rbegin();
				RobotPart *lastPart = *rb_iterator;

				while ((target_point - *lastPart->getEndPoint()).dis_from_origin()
						> target_threshold) {
					for (auto part = robotParts.rbegin();
							part != robotParts.rend() - 1; ++part) {
						this_thread::sleep_for(std::chrono::milliseconds(120));
						RobotPart *current_part = *part;
						Line line_2_target(*current_part->getFixedPoint(),
								target_point);
						Line line_2_TCP(*current_part->getFixedPoint(),
								*lastPart->getEndPoint());

						if (current_part->getJointType() == PRISMATIC) {
							float move_angle = line_2_target.angles().first
									- line_2_TCP.angles().first;
							current_part->setRelativeAngle(
									current_part->getRelativeAngle()
											+ move_angle);
						} else if (current_part->getJointType() == PRISMATIC) {
							float move_angle = line_2_target.angles().second
									- line_2_TCP.angles().second;
							current_part->setRelativeAngle(
									current_part->getRelativeAngle()
											+ move_angle);
						} else {
							std::pair<float, float> move_angles = make_pair(
									current_part->getCurrentAngles().first
											+ line_2_target.angles().first
											- line_2_TCP.angles().first,
									current_part->getCurrentAngles().second
											+ line_2_target.angles().second
											- line_2_TCP.angles().second);
							current_part->setCurrentAngles(move_angles);
						}
//						cout << line_2_target << endl;
					}
					count++;
//					cout << count;
				}

			});

	thred1.join();
	cout << endl << endl << "Robot Angles..." << endl;
	cout << *this;
}

const Coordinate& MyRobot::getTargetPoint() const {
	return target_point;
}

void MyRobot::achieve_TCP(Coordinate target) {
	target_point = target;
	perform_inverse_kinematics();
}

void MyRobot::setTargetPoint(const Coordinate &targetPoint) {
	target_point = targetPoint;
}

void MyRobot::setOnChange(const std::function<void(void)> &onChange) {
	this->onChange = onChange;
}

std::ostream& operator<<(std::ostream &os, MyRobot &obj) {
	for (RobotPart *robotPart : obj.getRobotParts()) {

		cout << robotPart->getName() << " => "
				<< r2d(robotPart->getRelativeAngle()) << endl;
	}

	RobotPart *lastPart = obj.getRobotPart_index(
			obj.getRobotParts().size() - 1);
	cout << "TCP: " << *lastPart->getEndPoint() << endl;
	return os;
}
