/*
 * TopViewWidget.h
 *
 *  Created on: Apr 9, 2025
 *      Author: vagha
 */

#ifndef TOPVIEWWIDGET_H_
#define TOPVIEWWIDGET_H_
#include <functional>
#include <QWidget>
#include <vector>

#include "../Shapes/Shape.h"
using std::function;
using std::vector;

class TopViewWidget: public QWidget {
private:
	int m_gridInterval;
	QColor m_gridColor;
	QColor m_axisColor;
	vector<Shape*> shapes;

	void drawGrid(QPainter &painter);
	void drawAxes(QPainter &painter);

public:
	explicit TopViewWidget(QWidget *parent = nullptr);
	virtual ~TopViewWidget();

	void setGridInterval(int interval);
	void setGridColor(const QColor &color);
	void setAxisColor(const QColor &color);
	void addShape(Shape *shape, function<void(QPainter&)> painterTransform =
			nullptr);

protected:
	void paintEvent(QPaintEvent *event) override;
};

#endif /* TOPVIEWWIDGET_H_ */
