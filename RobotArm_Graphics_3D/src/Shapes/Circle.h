/*
 * Circle.h
 *
 *  Created on: Mar 30, 2025
 *      Author: vagha
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_
#include "../Coordinate.h"
#include "Shape.h"
#include "Line.h"

class Circle: public Shape{
private:
	float radius;
	Coordinate center;
	Coordinate bottomLeft;

	void updateBottomLeft();

public:
	Circle(Coordinate center, float radius);
	virtual ~Circle();
	Coordinate closestPoint_2_origin() const;
	virtual void draw(QPainter &painter, View viewType) const override;
};

#endif /* CIRCLE_H_ */
