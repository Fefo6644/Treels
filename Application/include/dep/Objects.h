#ifndef _OBJECTS_H_
#define _OBJECTS_H_

namespace objs {
	typedef struct Color {
		Color(float r, float g, float b, float a);

		D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	} Color;

	typedef struct Point {
		Point(float x, float y);

		D2D1_POINT_2F point = D2D1::Point2F(0.0f, 0.0f);
	} Point;

	typedef struct Circle {
		Circle(Color colorFill = Color(0.0f, 0.0f, 0.0f, 1.0f), Color colorOutline = Color(0.0f, 0.0f, 0.0f, 1.0f), Point center = Point(0.0f, 0.0f), float radius = 10.0f, short zOrder = 0);

		bool drawFill = true;
		bool drawOutline = true;
		Color colorFill = Color(1.0f, 1.0f, 1.0f, 1.0f);
		Color colorOutline = Color(0.0f, 0.0f, 0.0f, 1.0f);
		D2D1_ELLIPSE circle = D2D1::Ellipse(D2D1::Point2F(), 10.0f, 10.0f);
	} Circle;

	typedef struct Rectangle {
		Rectangle(Color colorFill = Color(0.0f, 0.0f, 0.0f, 1.0f), Color colorOutline = Color(0.0f, 0.0f, 0.0f, 1.0f), Point fromCorner = Point(0.0f, 0.0f), Point toCorner = Point(0.0f, 0.0f), short zOrder = 0);

		bool drawFill = true;
		bool drawOutline = true;
		Color colorFill = Color(1.0f, 1.0f, 1.0f, 1.0f);
		Color colorOutline = Color(0.0f, 0.0f, 0.0f, 1.0f);
		D2D1_RECT_F rectangle = D2D1::RectF();
	} Rectangle;

	typedef struct Line {
		Line(Color color = Color(0.0f, 0.0f, 0.0f, 1.0f), Point from = Point(0.0f, 0.0f), Point to = Point(0.0f, 0.0f), short zOrder = 0);

		bool draw = true;
		Color color = Color(0.0f, 0.0f, 0.0f, 1.0f);
		Point from = Point(0.0f, 0.0f);
		Point to = Point(0.0f, 0.0f);
	} Line;
}

#endif // !_OBJECTS_H_
