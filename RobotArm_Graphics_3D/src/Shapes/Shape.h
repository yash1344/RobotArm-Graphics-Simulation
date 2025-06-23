/*
 * Shape.h
 *
 *  Created on: Apr 5, 2025
 *      Author: vagha
 */

#ifndef SHAPES_SHAPE_H_
#define SHAPES_SHAPE_H_
#pragma once
#include <QPainter>

enum View {
	TOP_VIEW, SIDE_VIEW
};

class Shape {
protected:
	std::function<void(QPainter&)> painterTransform;

public:
	Shape();
	virtual ~Shape();

	virtual void draw(QPainter &painter, View viewType) const = 0;
	void setPainterTransform(std::function<void(QPainter&)> transform);

};

#endif /* SHAPES_SHAPE_H_ */
