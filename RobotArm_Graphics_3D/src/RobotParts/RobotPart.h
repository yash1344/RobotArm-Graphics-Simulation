/*
 * RobotPart.h
 *
 *  Created on: Apr 9, 2025
 *      Author: vagha
 */

#ifndef ROBOTPARTS_ROBOTPART_H_
#define ROBOTPARTS_ROBOTPART_H_
#include <qpainter.h>
#include <iostream>
#include <string>

#include "../Coordinate.h"
#include "../Shapes/Shape.h"

enum JointType {
	REVOLUTE, PRISMATIC, OTHERS
};

using std::ostream;
class RobotPart {
private:
	std::string name;
	JointType jointType;
	float length;
	float relative_angle;
	std::pair<float, float> current_angle;
	float min_angle, max_angle;
	const RobotPart *holder_robotPart;
	const Coordinate *fixed_point;
	Coordinate *end_point;
	QColor color;
	std::function<void(QPainter&, RobotPart&)> onPartDraw_TopView;
	std::function<void(QPainter&, RobotPart&)> onPartDraw_SideView;
	std::function<void(void)> onChange;

	RobotPart& updatePosition();

public:
	RobotPart(std::string name, JointType jointType, float relative_angle,
			float length, float min_angle, float max_angle,
			const RobotPart *holder_robotPart, QColor color,
			std::function<void(void)> onChange);

	RobotPart(std::string name, JointType jointType,
			std::pair<float, float> current_angle, float length,
			float min_angle, float max_angle, const Coordinate *fixed_point,
			QColor color, std::function<void(void)> onChange);
	virtual ~RobotPart();

	void drawPart(QPainter &painter, View viewType);

	std::string printCurrentStatus() const;
	std::string printRange() const;

	const std::pair<float, float>& getCurrentAngles() const;
	void setCurrentAngles(const std::pair<float, float> &currentAngles);
	JointType getJointType() const;
	void setOnPartDrawSideView(
			const std::function<void(QPainter&, RobotPart&)> &onPartDrawSideView);
	void setOnPartDrawTopView(
			const std::function<void(QPainter&, RobotPart&)> &onPartDrawTopView);
	float getLength() const;
	void setLength(float length);
	const std::string& getName() const;
	void setName(const std::string &name);
	float getMaxAngle() const;
	void setMaxAngle(float maxAngle);
	float getMinAngle() const;
	void setMinAngle(float minAngle);
	const Coordinate* const getEndPoint() const;
	const Coordinate* const getFixedPoint() const;
	void setFixedPoint(const Coordinate *const fixedPoint);
	const RobotPart* getHolderRobotPart() const;
	void setHolderRobotPart(const RobotPart *&holderRobotPart);
	const QColor& getColor() const;
	void setColor(const QColor &color);
	float getRelativeAngle() const;
	void setRelativeAngle(float relativeAngle);
	void setOnPartDraw(
			const std::function<void(QPainter&, RobotPart&)> &onPartDraw,
			View viewType, bool isBothViewSame);

	friend ostream& operator<<(ostream &out, RobotPart &part);
	void setOnChange(const std::function<void(void)> &onChange);

	friend std::ostream& operator<<(std::ostream &os, const RobotPart &obj);
};

#endif /* ROBOTPARTS_ROBOTPART_H_ */
