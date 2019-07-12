#include "pch.h"

#include "Objects.h"

namespace Treels {
	namespace Objects {
		/*---------------- OBJECT ----------------*/

		Object::Object(const Type t) : _guid(CreateGuid()), type(t) {}

		GUID Object::CreateGuid() {
			GUID guid = { 0 };
			HRESULT hres = CoCreateGuid(&guid);
			if (FAILED(hres)) {
				::std::wstringstream err;
				err << L"Failed to create object. Program will shut down. Error code: " << ::std::hex << hres;
				MessageBox(NULL, err.str().c_str(), L"Fatal Error", MB_OK | MB_ICONERROR);
				::std::exit(-1);
			}
			return guid;
		}

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

		Circle::Circle(Color colorFill, Color colorOutline, Point center, float radius, short zOrder) : Object(Type::ECircle) {
			circle.point = center.point;
			circle.radiusX = circle.radiusY = radius;
			this->colorFill = colorFill;
			this->colorOutline = colorOutline;
			drawFill = drawOutline = true;
			this->zOrder = zOrder;
		}

		/*---------------- RECTANGLE ----------------*/

		Rectangle::Rectangle(Color colorFill, Color colorOutline, Point fromCorner, Point toCorner, short zOrder) : Object(Type::ERectangle) {
			rectangle.left = fromCorner.point.x;
			rectangle.top = fromCorner.point.y;
			rectangle.right = toCorner.point.x;
			rectangle.bottom = toCorner.point.y;
			this->colorFill = colorFill;
			this->colorOutline = colorOutline;
			drawFill = drawOutline = true;
			this->zOrder = zOrder;
		}

		/*---------------- LINE ----------------*/

		Line::Line(Color color, Point from, Point to, short zOrder) : Object(Type::ELine) {
			draw = true;
			this->color = color;
			this->from = from;
			this->to = to;
			this->zOrder = zOrder;
		}
	}
}