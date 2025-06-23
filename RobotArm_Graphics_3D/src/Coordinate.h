/*
 * Coordinate.h
 *
 *  Created on: Mar 30, 2025
 *      Author: vagha
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_
#include <cmath>
#include <iostream>

class Coordinate {
private:
	float x, y, z;

public:
	Coordinate();
	Coordinate(float x, float y, float z);
	virtual ~Coordinate();
	float X() const;
	void setX(float x);
	float Y() const;
	void setY(float y);
	float Z() const;
	void setZ(float z);

	float dis_from_origin() const;

	Coordinate operator+(const Coordinate &other) const;
	Coordinate operator-(const Coordinate &other) const;
	Coordinate operator/(const float scale) const;
	Coordinate operator*(const float scale) const;
	bool operator==(const Coordinate &other) const;
	bool operator!=(const Coordinate &other) const;
	bool operator<(const Coordinate &other) const;
	bool operator<=(const Coordinate &other) const;
	bool operator>(const Coordinate &other) const;
	bool operator>=(const Coordinate &other) const;
	friend std::ostream& operator<<(std::ostream &os, const Coordinate &coord);
};

#endif /* COORDINATE_H_ */
