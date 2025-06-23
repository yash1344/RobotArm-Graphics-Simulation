/*
 * RobotPart.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: vagha
 */

#include <iostream>
#include <sstream>
#include "../Shapes/Line.h"
#include "../Shapes/Circle.h"
#include "../Shapes/Rectangle.h"
#include "RobotPart.h"

#include "../Utility.h"
using namespace std;
RobotPart::RobotPart(std::string name, JointType jointType,
		float relative_angle, float length, float min_angle, float max_angle,
		const RobotPart *holder_robotPart, QColor color,
		std::function<void(void)> onChange) :
		name(name), jointType(jointType), length(length), relative_angle(
				limit(min_angle, relative_angle, max_angle)), min_angle(
				min_angle), max_angle(max_angle), holder_robotPart(
				holder_robotPart), fixed_point(holder_robotPart->getEndPoint()), end_point(
				nullptr), color(color), onChange(onChange) {
	updatePosition();
}

RobotPart::RobotPart(std::string name, JointType jointType,
		std::pair<float, float> current_angle, float length, float min_angle,
		float max_angle, const Coordinate *fixed_point, QColor color,
		std::function<void(void)> onChange) :
		name(name), jointType(jointType), length(length), current_angle(
				current_angle), min_angle(min_angle), max_angle(max_angle), holder_robotPart(
				nullptr), fixed_point(fixed_point), end_point(nullptr), color(
				color), onChange(onChange) {
	if (jointType == PRISMATIC) {
		this->relative_angle = current_angle.first;
	} else {
		this->relative_angle = current_angle.second;
	}
	updatePosition();
}

RobotPart::~RobotPart() {
	holder_robotPart = nullptr;
	delete end_point;
}

std::string RobotPart::printCurrentStatus() const {
	ostringstream oss;

	if (holder_robotPart) {
		oss << name << ": " << relative_angle;
	} else {
		oss << name << ": (" << current_angle.first << ", "
				<< current_angle.second << ")";
	}
	return oss.str();
}

std::string RobotPart::printRange() const {
	return name + " range: " + to_string(min_angle) + " - "
			+ to_string(max_angle);
}

void RobotPart::setCurrentAngles(const std::pair<float, float> &currentAngles) {
	if (holder_robotPart) {
		if (jointType == PRISMATIC) {
			setRelativeAngle(
					currentAngles.first
							- holder_robotPart->getCurrentAngles().first);
		} else if (jointType == REVOLUTE) {
			setRelativeAngle(
					currentAngles.second
							- holder_robotPart->getCurrentAngles().second);
		} else {

		}
	}
	current_angle = currentAngles;
	updatePosition();
	onChange();
}

float RobotPart::getLength() const {
	return length;
}

void RobotPart::setLength(float length) {
	this->length = length;
	updatePosition();
	onChange();
}

const std::string& RobotPart::getName() const {
	return name;
}

void RobotPart::setName(const std::string &name) {
	this->name = name;
	onChange();
}

RobotPart& RobotPart::updatePosition() {
	if (holder_robotPart) {
		current_angle = holder_robotPart->getCurrentAngles();
		if (jointType == PRISMATIC) {
			current_angle.first += relative_angle;
		} else {
			current_angle.second += relative_angle;
		}
	}
	Line partLine(*fixed_point, START, current_angle.first,
			current_angle.second, length);

	if (end_point) {
		*end_point = Coordinate(partLine.getEnd());
	} else {
		end_point = new Coordinate(partLine.getEnd());
	}

	return *this;
}

const Coordinate* const RobotPart::getEndPoint() const {
	return end_point;
}

const Coordinate* const RobotPart::getFixedPoint() const {
	return this->fixed_point;
}

void RobotPart::setFixedPoint(const Coordinate *const fixedPoint) {
	this->fixed_point = fixedPoint;
	onChange();
}

const RobotPart* RobotPart::getHolderRobotPart() const {
	return holder_robotPart;
}

void RobotPart::setHolderRobotPart(const RobotPart *&holderRobotPart) {
	holder_robotPart = holderRobotPart;
	updatePosition();
	onChange();
}

float RobotPart::getRelativeAngle() const {
	return relative_angle;
}

void RobotPart::setRelativeAngle(float relativeAngle) {
	this->relative_angle = limit(min_angle, relativeAngle, max_angle);
	onChange();
}

void RobotPart::setColor(const QColor &color) {
	this->color = color;
	onChange();
}

void RobotPart::drawPart(QPainter &painter, View viewType) {
	updatePosition();
	painter.save();

	if (viewType == SIDE_VIEW) {
		if (onPartDraw_SideView) {
			onPartDraw_SideView(painter, *this);
		} else {
			Line line = Line(*fixed_point, *end_point);
			line.cut_or_extend(40, CENTER);
			Rectangle rect(line, 40);
			rect.setCornerRadius(20);
			rect.setPainterTransform([this](QPainter &painter) {
				// Custom pen (border)
				QPen pen;
				pen.setWidth(1);
				pen.setCapStyle(Qt::RoundCap);
				pen.setColor(QColorConstants::Black);
				painter.setPen(pen);

				// Custom brush (fill)
				QBrush brush(this->color);
				brush.setStyle(Qt::BrushStyle::SolidPattern);
				painter.setBrush(brush);
			});
			rect.draw(painter, SIDE_VIEW);
			painter.restore();

			painter.save();
			Circle fixedPointCircle(*fixed_point, 7);
			fixedPointCircle.setPainterTransform([](QPainter &painter) {
				// Custom pen (border)
				painter.setPen(Qt::NoPen);

				// Custom brush (fill)
				QBrush brush(QColor(0, 0, 0, 100));
				brush.setStyle(Qt::BrushStyle::SolidPattern);
				painter.setBrush(brush);
			});
			Circle endPointCircle(*end_point, 3);
			fixedPointCircle.setPainterTransform([](QPainter &painter) {
				// Custom pen (border)
				painter.setPen(Qt::NoPen);

				// Custom brush (fill)
				QBrush brush(QColor(0, 0, 0, 100));
				brush.setStyle(Qt::BrushStyle::SolidPattern);
				painter.setBrush(brush);
			});
			endPointCircle.draw(painter, SIDE_VIEW);
		}
	}

	//TODO update accordingly
	if (viewType == TOP_VIEW) {
		if (onPartDraw_SideView) {
			onPartDraw_SideView(painter, *this);
		} else {
			Line line = Line(*fixed_point, *end_point);
			line.cut_or_extend(40, CENTER);
			Rectangle rect(line, 40);
			rect.setCornerRadius(20);
			rect.setPainterTransform([this](QPainter &painter) {
				// Custom pen (border)
				QPen pen;
				pen.setWidth(1);
				pen.setCapStyle(Qt::RoundCap);
				pen.setColor(QColorConstants::Black);
				painter.setPen(pen);

				// Custom brush (fill)
				QBrush brush(this->color);
				brush.setStyle(Qt::BrushStyle::SolidPattern);
				painter.setBrush(brush);
			});
			rect.draw(painter, SIDE_VIEW);
			painter.restore();

			painter.save();
			Circle fixedPointCircle(*fixed_point, 7);
			fixedPointCircle.setPainterTransform([](QPainter &painter) {
				// Custom pen (border)
				painter.setPen(Qt::NoPen);

				// Custom brush (fill)
				QBrush brush(QColor(0, 0, 0, 100));
				brush.setStyle(Qt::BrushStyle::SolidPattern);
				painter.setBrush(brush);
			});
			Circle endPointCircle(*end_point, 3);
			fixedPointCircle.setPainterTransform([](QPainter &painter) {
				// Custom pen (border)
				painter.setPen(Qt::NoPen);

				// Custom brush (fill)
				QBrush brush(QColor(0, 0, 0, 100));
				brush.setStyle(Qt::BrushStyle::SolidPattern);
				painter.setBrush(brush);
			});
			endPointCircle.draw(painter, SIDE_VIEW);
		}
	}
	painter.restore();
}

ostream& operator<<(ostream &out, RobotPart &part) {
	out << part.getName() << "............" << endl;
	out << "RA: " << r2d(part.relative_angle) << " || CAs: ("
			<< r2d(part.current_angle.first) << ", "
			<< r2d(part.current_angle.second) << ")" << endl;
	return out;
}

float RobotPart::getMaxAngle() const {
	return max_angle;
}

void RobotPart::setMaxAngle(float maxAngle) {
	max_angle = maxAngle;
	updatePosition();
	onChange();
}

float RobotPart::getMinAngle() const {
	return min_angle;
}

void RobotPart::setMinAngle(float minAngle) {
	min_angle = minAngle;
	updatePosition();
	onChange();
}

void RobotPart::setOnChange(const std::function<void(void)> &onChange) {
	this->onChange = onChange;
}

void RobotPart::setOnPartDraw(
		const std::function<void(QPainter&, RobotPart&)> &onPartDraw,
		View viewType, bool isBothViewSame) {
	if (isBothViewSame) {
		this->onPartDraw_TopView = onPartDraw;
		this->onPartDraw_SideView = onPartDraw;
	} else {
		if (viewType == SIDE_VIEW) {
			this->onPartDraw_SideView = onPartDraw;
		} else {
			this->onPartDraw_TopView = onPartDraw;
		}
	}
}

const QColor& RobotPart::getColor() const {
	return color;
}

const std::pair<float, float>& RobotPart::getCurrentAngles() const {
	return current_angle;
}

JointType RobotPart::getJointType() const {
	return jointType;
}

void RobotPart::setOnPartDrawSideView(
		const std::function<void(QPainter&, RobotPart&)> &onPartDrawSideView) {
	onPartDraw_SideView = onPartDrawSideView;
}

void RobotPart::setOnPartDrawTopView(
		const std::function<void(QPainter&, RobotPart&)> &onPartDrawTopView) {
	onPartDraw_TopView = onPartDrawTopView;
}

std::ostream& operator<<(std::ostream &os, const RobotPart &obj) {
	os << obj.printCurrentStatus();
	return os;
}
