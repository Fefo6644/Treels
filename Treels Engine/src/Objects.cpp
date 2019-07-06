#include "pch.h"

#include "Objects.h"

namespace objs {
	/*---------------- COLOR ----------------*/

	Color::Color(float r, float g, float b, float a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	/*---------------- POINT ----------------*/

	Point::Point(float x, float y) {
		point.x = x;
		point.y = y;
	}

	/*---------------- CIRCLE ----------------*/

	Circle::Circle(Color colorFill, Color colorOutline, Point center, float radius, short zOrder) {
		circle.point = center.point;
		circle.radiusX = circle.radiusY = radius;
		this->colorFill = colorFill;
		this->colorOutline = colorOutline;
		drawFill = drawOutline = true;
	}

	/*---------------- RECTANGLE ----------------*/

	Rectangle::Rectangle(Color colorFill, Color colorOutline, Point fromCorner, Point toCorner, short zOrder) {
		rectangle.left = fromCorner.point.x;
		rectangle.top = fromCorner.point.y;
		rectangle.right = toCorner.point.x;
		rectangle.bottom = toCorner.point.y;
		this->colorFill = colorFill;
		this->colorOutline = colorOutline;
		drawFill = drawOutline = true;
	}

	/*---------------- LINE ----------------*/

	Line::Line(Color color, Point from, Point to, short zOrder) {
		draw = true;
		this->color = color;
		this->from = from;
		this->to = to;
	}
}
