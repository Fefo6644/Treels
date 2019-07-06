#define _USE_MATH_DEFINES
#include <cmath>

#include <dep/Treels API.h>

#include "Application.h"

bool reset = false;
bool inc = false;
bool dec = false;
float multiplier = 0;
float windowWidth = 0.0f;
float windowHeight = 0.0f;

TreelsMainWindow* treelsWindow;

objs::Point center(0.0f, 0.0f);
objs::Color lineColor(0.0f, 0.0f, 0.0f, 1.0f);
objs::Circle ball;
std::vector<objs::Line> lines;
std::vector<objs::Line*> pLines;
std::vector<objs::Circle*> pBalls;

void AppSetup(TreelsMainWindow* applicationWindow) {
	treelsWindow = applicationWindow;
	treelsWindow->KeyPress = KeyPress;
	treelsWindow->Resize = Resize;
	treelsWindow->Closing = Closing;

	treelsWindow->GetWindowSize(&windowWidth, &windowHeight);
	treelsWindow->SetBackgroundColor(0.75f, 0.75f, 0.75f);

	ball.colorOutline = objs::Color(0.0f, 0.0f, 0.0f, 1.0f);
	ball.drawFill = false;
	ball.drawOutline = true;
	ball.circle.radiusX = ball.circle.radiusY = windowWidth < windowHeight ? windowWidth : windowHeight;
	ball.circle.point = center.point;

	pBalls.push_back(&ball);

	objs::Line lineTemp = objs::Line();
	lineTemp.draw = true;
	lines = std::vector<objs::Line>(360);
	float x, y, rad;
	for (size_t i = 0; i < lines.size(); ++i) {
		rad = i / 180.0f * (float)M_PI;
		x = center.point.x + std::cosf(rad) * ball.circle.radiusX;
		y = center.point.y - std::sinf(rad) * ball.circle.radiusY;

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

		lineTemp.color = lineColor;
		lineTemp.from.point.x = lineTemp.to.point.x = x;
		lineTemp.from.point.y = lineTemp.to.point.y = y;

		lines[i] = lineTemp;
		pLines.push_back(&lines[i]);
	}

	treelsWindow->LoadCircles(&pBalls);
	treelsWindow->LoadLines(&pLines);
}

void AppLoop() {
	if (reset)
		multiplier = 0.0f;
	if (inc)
		multiplier = multiplier < 25.0f ? multiplier + 0.01f : 0.0f;
	if (dec)
		multiplier = multiplier > 0.0f ? multiplier - 0.01f : 25.0f;

	objs::Line lineTemp = objs::Line();
	lineTemp.draw = true;
	float rad;
	for (int i = 0; i < 360; ++i) {
		rad = i / 180.0f * (float)M_PI;
		lines[i].from.point.x = center.point.x + std::cosf(rad) * ball.circle.radiusX;
		lines[i].from.point.y = center.point.y - std::sinf(rad) * ball.circle.radiusY;

		lines[i].to.point.x = center.point.x + std::cosf(rad * multiplier) * ball.circle.radiusX;
		lines[i].to.point.y = center.point.y - std::sinf(rad * multiplier) * ball.circle.radiusY;
	}
}

void KeyPress(Action action, Key key) {
	if (key == Key::EscapeKey)
		treelsWindow->Close();
	if (key == Key::R)
		reset = action;
	if (key == Key::UpArrow)
		inc = action;
	if (key == Key::DownArrow)
		dec = action;
}

void Resize(float newWidth, float newHeight) {
	windowWidth = newWidth;
	windowHeight = newHeight;

	center.point.x = windowWidth / 2.0f;
	center.point.y = windowHeight / 2.0f;
	ball.circle.point = center.point;
	ball.circle.radiusX = ball.circle.radiusY = (windowWidth < windowHeight ? windowWidth : windowHeight) / 2.0f;

	objs::Line lineTemp = objs::Line();
	lineTemp.draw = true;
	float rad;
	for (size_t i = 0; i < lines.size(); ++i) {
		rad = i / 180.0f * (float)M_PI;
		lines[i].from.point.x = center.point.x + std::cosf(rad) * ball.circle.radiusX;
		lines[i].from.point.y = center.point.y - std::sinf(rad) * ball.circle.radiusY;

		lines[i].to.point.x = center.point.x + std::cosf(rad + rad * multiplier) * ball.circle.radiusX;
		lines[i].to.point.y = center.point.y - std::sinf(rad + rad * multiplier) * ball.circle.radiusY;
	}
}

void Closing() {
	treelsWindow->UnloadCircles();
	treelsWindow->UnloadLines();
}
