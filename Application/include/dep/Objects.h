#ifndef _OBJECTS_H_
#define _OBJECTS_H_

namespace objs {

	typedef struct Color {
	public:
		Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

		float GetR(void) const;
		float GetG(void) const;
		float GetB(void) const;
		float GetA(void) const;

		D2D1_COLOR_F GetD2D1Color(void) const;

		void SetR(float r);
		void SetG(float g);
		void SetB(float b);
		void SetA(float a);

	private:
		D2D1_COLOR_F color;
		float _r;
		float _g;
		float _b;
		float _a;
	} Color;

	typedef struct Point {
	public:
		Point(float x = 0.0f, float y = 0.0f);

		float GetX(void) const;
		float GetY(void) const;

		D2D1_POINT_2F GetD2D1Point(void) const;

		void SetX(float x);
		void SetY(float y);

	private:
		D2D1_POINT_2F point;
		float _x;
		float _y;
	} Point;

	typedef struct Circle {
	public:
		Circle(Color colorFill = Color(), Color colorOutline = Color(), Point center = Point(), float radius = 10.0f);

		bool drawFill;
		bool drawOutline;
		Color colorFill;
		Color colorOutline;

		float GetRadius(void) const;
		Point GetCenter(void) const;

		D2D1_ELLIPSE GetD2D1Ellipse(void) const;

		void SetRadius(float radius);
		void SetCenter(Point center);

	private:
		D2D1_ELLIPSE circle;
		Point _center;
		float _radius;
	} Circle;

	typedef struct Rectangle {
	public:
		Rectangle(Color colorFill = Color(), Color colorOutline = Color(), Point fromCorner = Point(), Point toCorner = Point());

		bool drawFill;
		bool drawOutline;
		Color colorFill;
		Color colorOutline;

		Point GetFromCorner(void) const;
		Point GetToCorner(void) const;

		D2D1_RECT_F GetD2D1Rect(void) const;

		void SetFromCorner(Point fromCorner);
		void SetToCorner(Point toCorner);

	private:
		D2D1_RECT_F rectangle;
		Point _fromCorner;
		Point _toCorner;
	} Rectangle;

	typedef struct Line {
	public:
		Line(Color color = Color(), Point from = Point(), Point to = Point());

		bool draw;
		Color color;
		Point from;
		Point to;
	} Line;

}

#endif // !_OBJECTS_H_
