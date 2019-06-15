#include "pch.h"

#include "Objects.h"

namespace objs {
	/*---------------- COLOR ----------------*/

	Color::Color(float r, float g, float b, float a) {
		_r = r;
		_g = g;
		_b = b;
		_a = a;

		color = D2D1::ColorF(r, g, b, a);
	}

	float Color::GetR(void) const { return _r; }
	float Color::GetG(void) const { return _g; }
	float Color::GetB(void) const { return _b; }
	float Color::GetA(void) const { return _a; }

	D2D1_COLOR_F Color::GetD2D1Color(void) const { return color; }

	void Color::SetR(float r) {
		_r = r;
		color.r = r;
	}
	void Color::SetG(float g) {
		_g = g;
		color.g = g;
	}
	void Color::SetB(float b) {
		_b = b;
		color.b = b;
	}
	void Color::SetA(float a) {
		_a = a;
		color.a = a;
	}

	/*---------------- POINT ----------------*/

	Point::Point(float x, float y) {
		_x = x;
		_y = y;

		point = D2D1::Point2F(x, y);
	}

	float Point::GetX(void) const { return _x; }
	float Point::GetY(void) const { return _y; }

	D2D1_POINT_2F Point::GetD2D1Point(void) const { return point; }

	void Point::SetX(float x) {
		_x = x;
		point.x = x;
	}
	void Point::SetY(float y) {
		_y = y;
		point.y = y;
	}

	/*---------------- CIRCLE ----------------*/

	Circle::Circle(Color colorFill, Color colorOutline, Point center, float radius) {
		circle = D2D1::Ellipse(center.GetD2D1Point(), radius, radius);
		_radius = radius;
		_center = center;
		drawFill = true;
		drawOutline = true;
		this->colorFill = colorFill;
		this->colorOutline = colorOutline;
	}

	float Circle::GetRadius(void) const { return _radius; }
	Point Circle::GetCenter(void) const { return _center; }

	D2D1_ELLIPSE Circle::GetD2D1Ellipse(void) const { return circle; }

	void Circle::SetCenter(Point center) {
		_center = center;
		circle.point = center.GetD2D1Point();
	}
	void Circle::SetRadius(float radius) {
		_radius = radius;
		circle.radiusX = radius;
		circle.radiusY = radius;
	}

	/*---------------- RECTANGLE ----------------*/

	Rectangle::Rectangle(Color colorFill, Color colorOutline, Point fromCorner, Point toCorner) {
		rectangle = D2D1::RectF(fromCorner.GetX(), fromCorner.GetY(), toCorner.GetX(), toCorner.GetY());
		_fromCorner = fromCorner;
		_toCorner = toCorner;
		drawFill = true;
		drawOutline = true;
		this->colorFill = colorFill;
		this->colorOutline = colorOutline;
	}

	Point Rectangle::GetFromCorner(void) const { return _fromCorner; }
	Point Rectangle::GetToCorner(void) const { return _toCorner; }

	D2D1_RECT_F Rectangle::GetD2D1Rect(void) const { return rectangle; }

	void Rectangle::SetFromCorner(Point fromCorner) {
		rectangle.left = fromCorner.GetX();
		rectangle.top = fromCorner.GetY();
		_fromCorner = fromCorner;
	}

	void Rectangle::SetToCorner(Point toCorner) {
		rectangle.right = toCorner.GetX();
		rectangle.bottom = toCorner.GetY();
		_toCorner = toCorner;
	}

	/*---------------- LINE ----------------*/

	Line::Line(Color color, Point from, Point to) {
		this->color = color;
		this->from = from;
		this->to = to;
		draw = true;
	}
}