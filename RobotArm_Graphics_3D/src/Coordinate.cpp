/*
 * Coordinate.cpp
 *
 *  Created on: Mar 30, 2025
 *      Author: vagha
 */

#include "Coordinate.h"

Coordinate::Coordinate() :
		x(0), y(0), z(0) {
}

Coordinate::Coordinate(float x, float y, float z) :
		x(x), y(y), z(z) {
}

float Coordinate::X() const {
	return x;
}

void Coordinate::setX(float x) {
	this->x = x;
}

float Coordinate::Y() const {
	return y;
}

void Coordinate::setY(float y) {
	this->y = y;
}

void Coordinate::setZ(float z) {
	this->z = z;
}
float Coordinate::Z() const {
	return z;
}

Coordinate::~Coordinate() {
}

Coordinate Coordinate::operator +(const Coordinate &other) const {
	return Coordinate(x + other.X(), y + other.Y(), z + other.Z());
}

Coordinate Coordinate::operator -(const Coordinate &other) const {
	return Coordinate(x - other.X(), y - other.Y(), z - other.Z());
}

float Coordinate::dis_from_origin() const {
	return hypotf(hypotf(x, y), z);
}

Coordinate Coordinate::operator /(const float scale) const {
	return Coordinate(x / scale, y / scale, z / scale);
}

Coordinate Coordinate::operator *(const float scale) const {
	return Coordinate(x * scale, y * scale, z * scale);
}

bool Coordinate::operator ==(const Coordinate &other) const {
	return x == other.X() && y == other.Y() && z == other.Z();
}

bool Coordinate::operator !=(const Coordinate &other) const {
	return !(*this == other);
}

//TODO may cause some issue
bool Coordinate::operator <(const Coordinate &other) const {
	return this->dis_from_origin() < other.dis_from_origin();
}

bool Coordinate::operator <=(const Coordinate &other) const {
	return this->dis_from_origin() == other.dis_from_origin()
			|| this->dis_from_origin() < other.dis_from_origin();
}

bool Coordinate::operator >(const Coordinate &other) const {
	return !((*this == other) || (*this < other));
}

bool Coordinate::operator >=(const Coordinate &other) const {
	return (*this == other) || (*this > other);
}

std::ostream& operator<<(std::ostream &os, const Coordinate &coord) {
	os << "(" << coord.x << ", " << coord.y << ", " << coord.z << ")";
	return os;
}
