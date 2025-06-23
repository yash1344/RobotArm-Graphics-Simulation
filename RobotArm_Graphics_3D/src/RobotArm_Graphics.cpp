//============================================================================
// Name        : RobotArm_Graphics.cpp
// Author      : Yashkumar Vaghani
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <QApplication>
#include <QWidget>
#include <QPainter>

#include "MyRobot.h"
#include "RobotParts/RobotPart.h"
#include "Shapes/Circle.h"
#include "Shapes/Line.h"
#include "Shapes/Rectangle.h"
#include "Utility.h"
#include "Windows/GridWidget.h"
#include "Windows/TopViewWidget.h"
#include <thread>
using namespace std;

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	GridWidget gridWidget;
	TopViewWidget topViewWidget;

	gridWidget.setAcceptDrops(true);
	gridWidget.resize(800, 600);
	gridWidget.setWindowTitle("Side View");

	gridWidget.setGridInterval(50);
	gridWidget.setGridColor(Qt::lightGray);
	gridWidget.setAxisColor(Qt::black);

	topViewWidget.setAcceptDrops(true);
	topViewWidget.resize(800, 600);
	topViewWidget.setWindowTitle("Side View");

	topViewWidget.setGridInterval(30);
	topViewWidget.setGridColor(Qt::lightGray);
	topViewWidget.setAxisColor(Qt::black);

	topViewWidget.show();
	gridWidget.show();

	const Coordinate *origin = new Coordinate(0, 0, 0);
	MyRobot *m_robot = new MyRobot();
	auto stage = m_robot->addRobotPart(
			new RobotPart("Stage", PRISMATIC, make_pair(d2r(90), d2r(90)), 100,
					d2r(90), d2r(90), origin, QColor(231, 76, 60, 50),
					[&gridWidget]() {
						gridWidget.update();
					}));
	stage->setOnPartDraw([](QPainter &painter, RobotPart &part) {
		Line line = Line(*part.getFixedPoint(), *part.getEndPoint());
		line.cut_or_extend(30, END);
		Rectangle rect(line, line.length());
		rect.setCornerRadius(5);
		rect.setPainterTransform([&part](QPainter &painter) {
			// Custom pen (border)
			QPen pen;
			pen.setWidth(1);
			pen.setCapStyle(Qt::RoundCap);
			pen.setColor(QColorConstants::Black);
			painter.setPen(pen);

			// Custom brush (fill)
			QBrush brush(part.getColor());
			brush.setStyle(Qt::BrushStyle::SolidPattern);
			painter.setBrush(brush);
		});
		painter.save();
		rect.draw(painter, SIDE_VIEW);
		painter.restore();
	}, SIDE_VIEW, true);

	m_robot->addRobotPart(
			new RobotPart("Base", PRISMATIC, d2r(0), 125, d2r(-180), d2r(180),
					m_robot->getRobotPart_index(0), QColor(46, 204, 113, 50),
					[&gridWidget]() {
						gridWidget.update();
					}));
	m_robot->addRobotPart(
			new RobotPart("Elbow", PRISMATIC, d2r(0), 90, d2r(-180), d2r(180),
					m_robot->getRobotPart_index(1), QColor(52, 152, 219, 50),
					[&gridWidget]() {
						gridWidget.update();
					}));
	m_robot->addRobotPart(
			new RobotPart("wrist", REVOLUTE, d2r(0), 30, d2r(0), d2r(0),
					m_robot->getRobotPart_index(2), QColor(241, 196, 15, 50),
					[&gridWidget]() {
						gridWidget.update();
					}));
	m_robot->addRobotPart(
			new RobotPart("Knuckle", PRISMATIC, d2r(0), 42, d2r(-45), d2r(45),
					m_robot->getRobotPart_index(3), QColor(155, 89, 182, 50),
					[&gridWidget]() {
						gridWidget.update();
					}));
	m_robot->addRobotPart(
			new RobotPart("Finger", OTHERS, d2r(0), 80, d2r(-45), d2r(45),
					m_robot->getRobotPart_index(4), QColor(26, 188, 156, 50),
					[&gridWidget]() {
						gridWidget.update();
					}));

	gridWidget.addRobot(m_robot);

	thread thread1([m_robot]() {
		this_thread::sleep_for(std::chrono::seconds(5));
		m_robot->achieve_TCP(Coordinate(250, 250, 0));
	});
	return app.exec();
}

//int main() {
//
//	Line line1(Coordinate(0, 0, 0), Coordinate(12, 14, 20));
//	cout << line1;
//	cout << endl << "line1 Normal angles => (" << line1.normal_angles_D().first
//			<< ", " << line1.normal_angles_D().second << ")";
//
//	Line line2(line1.centerPoint(), CENTER, line1.normal_angles().first,
//			line1.normal_angles().second, 10);
//	cout << endl << line2;
//	return 0;
//}
