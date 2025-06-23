/*
 * Circle.cpp
 *
 *  Created on: Mar 30, 2025
 *      Author: vagha
 */

#include "Circle.h"
Circle::Circle(Coordinate center, float radius) :
		radius(radius), center(center) {
	updateBottomLeft();
}

Circle::~Circle() {
}

Coordinate Circle::closestPoint_2_origin() const {
	Line line2Origin(Coordinate(0, 0, 0), center);
	return line2Origin.cut_or_extend(radius, END).getEnd();
}

void Circle::draw(QPainter &painter, View viewType) const {
	if (painterTransform) {
		painterTransform(painter);
	} else {
		// Custom brush (fill)
		QBrush brush(QColor(0, 0, 0, 50));
		brush.setStyle(Qt::BrushStyle::SolidPattern);
		painter.setBrush(brush);
	}

	//as painter is mirrored...
	if (viewType == SIDE_VIEW) {
		painter.drawEllipse(bottomLeft.X(), bottomLeft.Y(), radius * 2,
				radius * 2);
	} else {
		painter.drawEllipse(bottomLeft.X(), bottomLeft.Z(), radius * 2,
				radius * 2);
	}
}

void Circle::updateBottomLeft() {
	bottomLeft = Coordinate(center.X() - radius, center.Y() - radius,
			center.Z() - radius);
}

