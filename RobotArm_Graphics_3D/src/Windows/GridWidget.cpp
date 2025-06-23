#include "GridWidget.h"

#include <qfont.h>
#include <qlabel.h>
#include <algorithm>
#include <cmath>
#include <QPainter>
#include <QPen>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
GridWidget::GridWidget(QWidget *parent) :
		QWidget(parent), m_gridInterval(50), m_gridColor(QColor(200, 200, 200)), m_axisColor(
				Qt::black), m_originOffset(0, 0), m_scale(1.0), m_isDragging(
				false), robot(nullptr) {
	setMouseTracking(true);
	setCursor(Qt::OpenHandCursor);

	// Set background to white by default
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
	setPalette(pal);

	label1 = new QLabel("Hello GridWidget!", this);
	label1->move(20, 20); // position inside the widget
	label1->setStyleSheet("color: black; font-size: 16px;");
	label1->show();
}

GridWidget::~GridWidget() {
	for (Shape *shape : shapes) {
		delete shape;  // Delete each object
	}
	shapes.clear();   // Clear the vector
}

void GridWidget::setGridInterval(int interval) {
	if (m_gridInterval != interval && interval > 0) {
		m_gridInterval = interval;
		update();
	}
}

void GridWidget::setGridColor(const QColor &color) {
	if (m_gridColor != color) {
		m_gridColor = color;
		update();
	}
}

void GridWidget::setAxisColor(const QColor &color) {
	if (m_axisColor != color) {
		m_axisColor = color;
		update();
	}
}

void GridWidget::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event)
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);

	// Set coordinate system: origin at bottom-left, y-axis pointing up
	painter.translate(0, height());
	painter.scale(1, -1);

	drawGrid(painter); //grid must be first
	drawAxes(painter); //axis must be after grid

	for (RobotPart *part : robot->getRobotParts()) {
		part->drawPart(painter, SIDE_VIEW);
	}
}

void GridWidget::drawGrid(QPainter &painter) {
	painter.save();
	QPen gridPen(m_gridColor);
	gridPen.setStyle(Qt::DotLine);
	painter.setPen(gridPen);

	float grid_scale = fmod(m_scale, 0.5f) + 1.0f;

	// Vertical lines bottom (x-axis)
	for (int x = m_originOffset.x(); x >= 0; x -= m_gridInterval * grid_scale) {
		painter.drawLine(x, 0, x, height());
	}

	// Vertical lines top(x-axis)
	for (int x = m_originOffset.x(); x <= width();
			x += m_gridInterval * grid_scale) {
		painter.drawLine(x, 0, x, height());
	}

	// Horizontal lines right(y-axis)
	for (int y = -m_originOffset.y(); y >= 0;
			y -= m_gridInterval * grid_scale) {
		painter.drawLine(0, y, width(), y);
	}

	// Horizontal lines left(y-axis)
	for (int y = -m_originOffset.y(); y <= height();
			y += m_gridInterval * grid_scale) {
		painter.drawLine(0, y, width(), y);
	}
	painter.restore();
}

void GridWidget::drawAxes(QPainter &painter) {
	painter.save();
	QPen axisPen(m_axisColor);
	axisPen.setWidth(1);
	painter.setPen(axisPen);

	int x = m_originOffset.x() + m_gridInterval;
	int y = -m_originOffset.y() + m_gridInterval;

	painter.translate(x, y);

	// X-axis (horizontal)
	painter.drawLine(-x, 0, -x + width(), 0);

	// Y-axis (vertical)
	painter.drawLine(0, -y, 0, -y + height());

	painter.restore();
	painter.translate(x, y);
	painter.scale(m_scale, m_scale);
}

void GridWidget::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_isDragging = true;
		m_lastDragPos = event->pos();

		setCursor(Qt::ClosedHandCursor);
//		cout << endl << event->pos().x() << ", " << event->pos().y() << flush;
	} else if (event->button() == Qt::MiddleButton) {
		m_scale = 1.0;
		update();
	} else if (event->button() == Qt::RightButton) {
		m_originOffset = QPoint(0, 0);
		update();
	}
}

void GridWidget::mouseMoveEvent(QMouseEvent *event) {
	if (m_isDragging) {
		QPoint delta = event->pos() - m_lastDragPos;
		m_originOffset += delta;
		m_lastDragPos = event->pos();
		update();  // Trigger repaint
	}
}

void GridWidget::addShape(Shape *shape,
		function<void(QPainter&)> painterTransform) {
	if (painterTransform) {
		shape->setPainterTransform(painterTransform);
	}
	shapes.push_back(shape);
	update();
}

void GridWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_isDragging = false;
		setCursor(Qt::OpenHandCursor);
	}
}

QPointF GridWidget::toLogicalCoords(const QPoint &screenPos) const {
	return QPointF((screenPos.x() - m_originOffset.x()),
			-(screenPos.y() - m_originOffset.y() - height()));
}

void GridWidget::wheelEvent(QWheelEvent *event) {
	if (event->angleDelta().y() > 0) {
		// Zoom in
		m_scale += 0.1;
	} else {
		// Zoom out
		m_scale -= 0.1;
	}

	m_scale = max(0.1f, m_scale);

	update();
}

void GridWidget::addRobot(MyRobot *robot) {
	this->robot = robot;
}
