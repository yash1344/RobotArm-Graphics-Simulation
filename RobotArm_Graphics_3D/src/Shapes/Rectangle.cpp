/*
 * Rectangle.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: vagha
 */

#include "Rectangle.h"

#include <cmath>
#include <iostream>

using namespace std;

#include "../Utility.h"
Rectangle::~Rectangle() {
}

Rectangle::Rectangle(Coordinate leftTopCorner, int width, int height,
		float angle) :
		leftBottomCorner(leftTopCorner), width(width), height(height), angle(
				angle) {

}

Rectangle::Rectangle(Line line, int height) :
		width(line.length()), height(height), angle(line.angles().first) {
	float x = line.getStart().X()
			- ((height / 2) * cos(line.angles().first - d2r(-90)));
	float y = line.getStart().Y()
			- ((height / 2) * sin(line.angles().first - d2r(-90)));
	leftBottomCorner = Coordinate(x, y, 0);
}

int Rectangle::getHeight() const {
	return height;
}

void Rectangle::setHeight(int height) {
	this->height = height;
}

const Coordinate& Rectangle::getLeftBottomCorner() const {
	return leftBottomCorner;
}

void Rectangle::setLeftBottomCorner(const Coordinate &leftTopCorner) {
	this->leftBottomCorner = leftTopCorner;
}

int Rectangle::getWidth() const {
	return width;
}

float Rectangle::getAngle() const {
	return angle;
}

void Rectangle::setAngle(float angle) {
	this->angle = angle;
}

void Rectangle::setCornerRadius(int cornerRadius = 0) {
	corner_radius = cornerRadius;
}

void Rectangle::setWidth(int width) {
	this->width = width;
}

void Rectangle::draw(QPainter &painter, View viewType) const {
	if (painterTransform) {
		painterTransform(painter);
	} else {
		// Custom brush (fill)
		QBrush brush(QColor(0, 0, 0, 50));
		brush.setStyle(Qt::BrushStyle::SolidPattern);
		painter.setBrush(brush);
	}

	painter.translate(leftBottomCorner.X(), leftBottomCorner.Y());
	painter.rotate((int) round(r2d(angle)));

	if (viewType == SIDE_VIEW) {
		painter.drawRoundedRect(0, 0, width, height, corner_radius,
				corner_radius, Qt::AbsoluteSize);
	} else {
		painter.drawRoundedRect(0, 0, width, height, corner_radius,
				corner_radius, Qt::AbsoluteSize);
	}
}

