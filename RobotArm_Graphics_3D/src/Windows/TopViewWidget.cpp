/*
 * TopViewWidget.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: vagha
 */

#include "TopViewWidget.h"

#include <qpainter.h>
#include <algorithm>
#include <thread>

#include "../Shapes/Circle.h"
#include "../Shapes/Line.h"
#include "../Utility.h"
using std::min;
TopViewWidget::TopViewWidget(QWidget *parent) :
		QWidget(parent), m_gridInterval(50), m_gridColor(QColor(200, 200, 200)), m_axisColor(
				Qt::black) {

	setMouseTracking(true);
	setCursor(Qt::OpenHandCursor);

	// Set background to white by default
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
	setPalette(pal);

}

TopViewWidget::~TopViewWidget() {
	for (Shape *shape : shapes) {
		delete shape;  // Delete each object
	}
	shapes.clear();   // Clear the vector
}

void TopViewWidget::setGridInterval(int interval) {
	if (m_gridInterval != interval && interval > 0) {
		m_gridInterval = interval;
		update();
	}
}

void TopViewWidget::setGridColor(const QColor &color) {
	if (m_gridColor != color) {
		m_gridColor = color;
		update();
	}
}

void TopViewWidget::setAxisColor(const QColor &color) {
	if (m_axisColor != color) {
		m_axisColor = color;
		update();
	}
}

void TopViewWidget::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event)
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);

	// Set coordinate system: origin at bottom-left, y-axis pointing up
	painter.translate(0, height());
	painter.scale(1, -1);

	drawGrid(painter); //grid must be first
	drawAxes(painter); //axis must be after grid

	for (const Shape *shape : shapes) {
		shape->draw(painter, TOP_VIEW);
	}
}
void TopViewWidget::drawGrid(QPainter &painter) {
	painter.save();

	float lineLength = min(height(), width()) * 0.4;
	for (int angle = 0; angle <= 360; angle += m_gridInterval) {
		Line grid_line(Coordinate(width() / 2, 0, height() / 2), START, 0,
				d2r(angle), lineLength);
		grid_line.setPainterTransform([this](QPainter &painter) {
			QPen gridPen(this->m_gridColor);
			gridPen.setStyle(Qt::DotLine);
			painter.setPen(gridPen);
		});
		grid_line.cut_or_extend(50, START);
		grid_line.draw(painter, TOP_VIEW);
	}

	for (int radius = m_gridInterval; radius <= lineLength; radius +=
			m_gridInterval) {
		Circle grid_circle(Coordinate(width() / 2, 0, height() / 2), radius);
		grid_circle.setPainterTransform([this](QPainter &painter) {
			// Custome Border
			QPen gridPen(this->m_gridColor);
			gridPen.setStyle(Qt::DotLine);
			painter.setPen(gridPen);

			// Custom brush (fill)
			QBrush brush;
			brush.setStyle(Qt::BrushStyle::NoBrush);
			painter.setBrush(brush);
		});
		grid_circle.draw(painter, TOP_VIEW);
	}
	painter.restore();
}

void TopViewWidget::drawAxes(QPainter &painter) {
	painter.save();
	QPen axisPen(m_axisColor);
	axisPen.setWidth(1);
	painter.setPen(axisPen);

	Line x_axe(Coordinate(width() / 2, 0, height() / 2), CENTER, 0, d2r(0),
			min(height(), width()) * 0.8);
	Line y_axe(Coordinate(width() / 2, 0, height() / 2), CENTER, 0, d2r(90),
			min(height(), width()) * 0.8);

// X-axis (horizontal)
	painter.drawLine(x_axe.getStart().X(), x_axe.getStart().Z(),
			x_axe.getEnd().X(), x_axe.getEnd().Z());

// Y-axis (vertical)
	painter.drawLine(y_axe.getStart().X(), y_axe.getStart().Z(),
			y_axe.getEnd().X(), y_axe.getEnd().Z());

	painter.restore();
}

void TopViewWidget::addShape(Shape *shape,
		function<void(QPainter&)> painterTransform) {
	if (painterTransform) {
		shape->setPainterTransform(painterTransform);
	}
	shapes.push_back(shape);
	update();
}
