/*
 * Line.h
 *
 *  Created on: Mar 30, 2025
 *      Author: vagha
 */

#ifndef LINE_H_
#define LINE_H_
#include <utility>

#include "../Coordinate.h"
#include "Shape.h"

enum direction {
	START, END, CENTER
};

class Line: public Shape {
	Coordinate start, end;
public:
	Line(Coordinate start, Coordinate end);
	Line(Coordinate anchorPoint, direction anchorPointType, float x_angle_rad,
			float z_angle_rad, float length);
	virtual ~Line();
	void draw(QPainter &painter, View viewType) const override;
	Line projection(View viewType) const;

	float length() const;
	Coordinate centerPoint() const;
	std::pair<float, float> angles() const;
	std::pair<float, float> angles_D() const;
	std::pair<float, float> normal_angles() const;
	std::pair<float, float> normal_angles_D() const;

	Line& cut_or_extend(float byLength, direction fromWhere);

	const Coordinate& getEnd() const;
	void setEnd(const Coordinate &end);
	const Coordinate& getStart() const;
	void setStart(const Coordinate &start);
	friend std::ostream& operator<<(std::ostream &os, const Line &line);
};

#endif /* LINE_H_ */
