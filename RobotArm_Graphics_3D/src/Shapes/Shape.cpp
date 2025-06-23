/*
 * Shape.cpp
 *
 *  Created on: Apr 5, 2025
 *      Author: vagha
 */

#include "Shape.h"

Shape::Shape() {

}

Shape::~Shape() {
}

void Shape::setPainterTransform(std::function<void(QPainter&)> transform) {
	painterTransform = transform;
}
