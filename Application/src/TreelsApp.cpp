#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <dep/Treels API.h>

#include "TreelsApp.h"

size_t TreelsApplication::GetStructSize() { return sizeof(TreelsApplication); }

TreelsApplication::TreelsApplication(Treels::TreelsEngine* applicationWindow) {
	window = applicationWindow;
	window->KeyPress = &TreelsApplication::KeyPress;
	window->Resize = &TreelsApplication::Resize;
	window->Closing = &TreelsApplication::Closing;

	window->GetWindowSize(windowWidth, windowHeight);
	window->SetBackgroundColor(0.75f, 0.75f, 0.75f);

	center.point.x = windowWidth / 2.0f;
	center.point.y = windowHeight / 2.0f;

	pBalls->push_back(Treels::Objects::Circle());
	(*pBalls)[0].colorOutline = Treels::Objects::Color(0.0f, 0.0f, 0.0f, 1.0f);
	(*pBalls)[0].drawFill = false;
	(*pBalls)[0].drawOutline = true;
	(*pBalls)[0].circle.point = center.point;
	(*pBalls)[0].circle.radiusX = (*pBalls)[0].circle.radiusY = (windowWidth < windowHeight ? windowWidth : windowHeight) / 2.0f;
	(*pBalls)[0].zOrder = INT16_MAX;

	pLines->reserve(360);
	Treels::Objects::Line temp;
	float x, y, rad;
	for (size_t i = 0; i < pLines->capacity(); ++i) {
		rad = i / 180.0f * (float)M_PI;
		x = center.point.x + ::std::cosf(rad) * (*pBalls)[0].circle.radiusX;
		y = center.point.y - ::std::sinf(rad) * (*pBalls)[0].circle.radiusY;

		if (i >= 0 && i < 60) {
			lineColor.color.r = 1.0f;
			lineColor.color.g = i / 59.0f;
			lineColor.color.b = 0.0f;
		}
		else if (i >= 60 && i < 120) {
			lineColor.color.r = 1.0f - (i - 60) / 59.0f;
			lineColor.color.g = 1.0f;
			lineColor.color.b = 0.0f;
		}
		else if (i >= 120 && i < 180) {
			lineColor.color.r = 0.0f;
			lineColor.color.g = 1.0f;
			lineColor.color.b = (i - 120) / 59.0f;
		}
		else if (i >= 180 && i < 240) {
			lineColor.color.r = 0.0f;
			lineColor.color.g = 1.0f - (i - 180) / 59.0f;
			lineColor.color.b = 1.0f;
		}
		else if (i >= 240 && i < 300) {
			lineColor.color.r = (i - 240) / 59.0f;
			lineColor.color.g = 0.0f;
			lineColor.color.b = 1.0f;
		}
		else if (i >= 300 && i < 360) {
			lineColor.color.r = 1.0f;
			lineColor.color.g = 0.0f;
			lineColor.color.b = 1.0f - (i - 300) / 59.0f;
		}

		temp.draw = true;
		temp.zOrder = (short)i + 1;
		temp.color = lineColor;
		temp.from.point.x = temp.to.point.x = x;
		temp.from.point.y = temp.to.point.y = y;

		pLines->push_back(temp);
	}

	window->LoadCircles(pBalls);
	window->LoadLines(pLines);
	window->Refresh();
}

void TreelsApplication::TreelsRun() {
	if (reset)
		multiplier = 0.0f;
	if (inc)
		multiplier += shifting ? 0.0001f : speeding ? 1.0f : 0.01f;
	if (dec)
		multiplier -= shifting ? 0.0001f : speeding ? 1.0f : 0.01f;

	if (multiplier > 180.0f)
		multiplier -= 360.0f;
	if (multiplier < -180.0f)
		multiplier += 360.0f;

	float rad;
	for (int i = 0; i < pLines->size(); ++i) {
		rad = i / 180.0f * (float)M_PI;
		(*pLines)[i].from.point.x = center.point.x + ::std::cosf(rad) * (*pBalls)[0].circle.radiusX;
		(*pLines)[i].from.point.y = center.point.y - ::std::sinf(rad) * (*pBalls)[0].circle.radiusY;

		(*pLines)[i].to.point.x = center.point.x + ::std::cosf(rad * multiplier) * (*pBalls)[0].circle.radiusX;
		(*pLines)[i].to.point.y = center.point.y - ::std::sinf(rad * multiplier) * (*pBalls)[0].circle.radiusY;
	}
}

void TreelsApplication::KeyPress(Treels::Action action, Treels::Key key) {
	if (key == Treels::Key::EscapeKey)
		window->Close();
	if (key == Treels::Key::R)
		reset = action;
	if (key == Treels::Key::UpArrow)
		inc = action;
	if (key == Treels::Key::DownArrow)
		dec = action;
	if (key == Treels::Key::Shift)
		shifting = action;
	if (key == Treels::Key::Control)
		speeding = action;
}

void TreelsApplication::Resize(float newWidth, float newHeight) {
	windowWidth = newWidth;
	windowHeight = newHeight;

	center.point.x = windowWidth / 2.0f;
	center.point.y = windowHeight / 2.0f;
	(*pBalls)[0].circle.point = center.point;
	(*pBalls)[0].circle.radiusX = (*pBalls)[0].circle.radiusY = (windowWidth < windowHeight ? windowWidth : windowHeight) / 2.0f;

	float rad;
	for (size_t i = 0; i < pLines->size(); ++i) {
		rad = i / 180.0f * (float)M_PI;
		(*pLines)[i].from.point.x = center.point.x + ::std::cosf(rad) * (*pBalls)[0].circle.radiusX;
		(*pLines)[i].from.point.y = center.point.y - ::std::sinf(rad) * (*pBalls)[0].circle.radiusY;

		(*pLines)[i].to.point.x = center.point.x + ::std::cosf(rad + rad * multiplier) * (*pBalls)[0].circle.radiusX;
		(*pLines)[i].to.point.y = center.point.y - ::std::sinf(rad + rad * multiplier) * (*pBalls)[0].circle.radiusY;
	}
}

void TreelsApplication::Closing() {
	window->Unload();
}
