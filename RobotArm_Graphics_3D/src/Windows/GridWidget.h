#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <functional>
#include <QWidget>
#include <QColor>
#include <qevent.h>
#include <qlabel.h>
#include <vector>

#include "../MyRobot.h"
#include "../Shapes/Shape.h"
using std::function;
using std::vector;

class GridWidget: public QWidget {

private:
	int m_gridInterval;
	QColor m_gridColor;
	QColor m_axisColor;
	QPoint m_originOffset;  // Stores panning offset
	QPoint m_lastDragPos;   // For tracking drag movement
	float m_scale;
	bool m_isDragging;      // Drag state flag
	vector<Shape*> shapes;
	MyRobot *robot;
	QLabel *label1;

	void drawGrid(QPainter &painter);
	void drawAxes(QPainter &painter);
	QPointF toLogicalCoords(const QPoint &screenPos) const;

public:
	explicit GridWidget(QWidget *parent = nullptr);
	virtual ~GridWidget();

	void addShape(Shape *shape, function<void(QPainter&)> painterTransform =
			nullptr);
	void addRobot(MyRobot *robot);

	void setGridInterval(int interval);
	void setGridColor(const QColor &color);
	void setAxisColor(const QColor &color);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
};

#endif // GRIDWIDGET_H
