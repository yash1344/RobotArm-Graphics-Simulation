/*
 * Line.cpp
 *
 *  Created on: Mar 30, 2025
 *      Author: vagha
 */

#include "Line.h"

#include "../Utility.h"
using namespace std;

Line::Line(Coordinate start, Coordinate end) {
//	if (start.dis_from_origin() < end.dis_from_origin()) {
	this->start = start;
	this->end = end;
//	} else {
//		this->start = end;
//		this->end = start;
//	}
}

Line::Line(Coordinate anchorPoint, direction anchorPointType, float x_angle_rad,
		float z_angle_rad, float length) {
	// 1) compute the two tan’s
	float tx = std::tan(x_angle_rad);
	float tz = std::tan(z_angle_rad);

//	// 2) find Δx that makes the total length == L
//	float invLen = 1.0f + tx * tx + tz * tz;
//	float dx = length / std::sqrt(invLen);
//	float dy = tx * dx;   // so atan2(dy,dx) == x_angle_rad
//	float dz = tz * dx;   // so atan2(dz,dx) == z_angle_rad
	float dx = length * cos(x_angle_rad) * cos(z_angle_rad);
	float dy = length * cos(z_angle_rad) * sin(x_angle_rad); // so atan2(dy,dx) == x_angle_rad
	float dz = length * cos(x_angle_rad) * sin(z_angle_rad); // so atan2(dz,dx) == z_angle_rad

	// 3) build the full offset
	Coordinate offset(dx, dy, dz);

	// 4) anchor start/end just like before
	switch (anchorPointType) {
	case START:
		start = anchorPoint;
		end = anchorPoint + offset;
		break;

	case END:
		end = anchorPoint;
		start = anchorPoint - offset;
		break;

	case CENTER: {
		Coordinate half = offset / 2.0f;
		start = anchorPoint - half;
		end = anchorPoint + half;
		break;
	}
	}
}

Line::~Line() {
}

std::pair<float, float> Line::angles() const {
	// 1) Compute vector delta
	float dx = end.X() - start.X();
	float dy = end.Y() - start.Y();
	float dz = end.Z() - start.Z();

	float xAngle = std::atan2(dy, dx);
	float zAngle = std::atan2(dz, dx);
	return {wrapAngleRad(xAngle), wrapAngleRad(zAngle)};
}

float Line::length() const {
	// 1) Compute vector delta
	float dx = end.X() - start.X();
	float dy = end.Y() - start.Y();
	float dz = end.Z() - start.Z();
	return hypotf(hypotf(dx, dy), dz);
}

const Coordinate& Line::getEnd() const {
	return end;
}

void Line::setEnd(const Coordinate &end) {
	this->end = end;
}

const Coordinate& Line::getStart() const {
	return start;
}

void Line::setStart(const Coordinate &start) {
	this->start = start;
}

Line& Line::cut_or_extend(float byLength, direction fromWhere) {
	// 1) compute the true delta
	float dx0 = end.X() - start.X();
	float dy0 = end.Y() - start.Y();
	float dz0 = end.Z() - start.Z();

	// 2) get its length and avoid degenerate case
	float len0 = std::sqrt(dx0 * dx0 + dy0 * dy0 + dz0 * dz0);
	if (len0 == 0.0f)      // zero‐length line → nothing to do
		return *this;

	// 3) form the unit direction = Δ / |Δ|
	float ux = dx0 / len0;
	float uy = dy0 / len0;
	float uz = dz0 / len0;

	// 4) scale by exactly the requested extension amount
	Coordinate offset(ux * byLength, uy * byLength, uz * byLength);

	// 5) apply to the chosen anchor
	switch (fromWhere) {
	case START:
		start = start - offset;
		break;
	case END:
		end = end + offset;
		break;
	case CENTER: {
		Coordinate half = offset / 2.0f;
		start = start - half;
		end = end + half;
		break;
	}
	}
	return *this;
}

std::pair<float, float> Line::normal_angles() const {
	auto [xAngle, zAngle] = this->angles();

	xAngle += d2r(90);
	zAngle += d2r(90);

	return {wrapAngleRad(xAngle), wrapAngleRad(zAngle)};
}

void Line::draw(QPainter &painter, View viewType) const {
	if (painterTransform) {
		painterTransform(painter);
	} else {
		// Custom pen (border)
		QPen pen;
		pen.setWidth(2);
		pen.setCapStyle(Qt::RoundCap);
		pen.setColor(QColorConstants::Black);
		painter.setPen(pen);
	}

	if (viewType == SIDE_VIEW) {
		painter.drawLine(start.X(), start.Y(), end.X(), end.Y());
	} else {
		painter.drawLine(start.X(), start.Z(), end.X(), end.Z());
	}
}

std::pair<float, float> Line::angles_D() const {
	auto angle_pair = angles();
	return {r2d(angle_pair.first), r2d(angle_pair.second)};
}

std::pair<float, float> Line::normal_angles_D() const {
	auto normal_angle_pair = normal_angles();
	return {r2d(normal_angle_pair.first), r2d(normal_angle_pair.second)};
}

std::ostream& operator<<(std::ostream &os, const Line &line) {
	os << "Line(Start: " << line.getStart() << ", End: " << line.getEnd()
			<< ") " << "|| length: " << line.length() << " || angles: ("
			<< line.angles_D().first << ", " << line.angles_D().second
			<< ")....   " << endl << line.getStart() << "," << line.getEnd();
	return os;
}

Coordinate Line::centerPoint() const {
	float dx0 = end.X() - start.X();
	float dy0 = end.Y() - start.Y();
	float dz0 = end.Z() - start.Z();

	return Coordinate(start.X() + dx0 / 2, start.Y() + dy0 / 2,
			start.Z() + dz0 / 2);
}

Line Line::projection(View viewType) const {

	auto proj_line = *this;
	if (viewType == SIDE_VIEW) {
		proj_line.start.setZ(0);
		proj_line.start.setZ(0);
	} else {
		proj_line.start.setY(0);
		proj_line.start.setY(0);
	}
	return proj_line;
}
