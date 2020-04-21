#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <dep/TreelsAPI.h>

#include "TreelsApp.h"

void TreelsApplication::setup() {
    getWindowSize(windowWidth, windowHeight);
    //setBackgroundColor(0.75f, 0.75f, 0.75f);

    center.x = windowWidth / 2.0f;
    center.y = windowHeight / 2.0f;

    pBalls.push_back(Treels::Objects::Circle());
    pBalls[0].colorOutline = Treels::Objects::Color(0.0f, 0.0f, 0.0f, 1.0f);
    pBalls[0].drawFill = false;
    pBalls[0].drawOutline = true;
    pBalls[0].center = center;
    pBalls[0].radiusX = pBalls[0].radiusY = (windowWidth < windowHeight ? windowWidth : windowHeight) / 2.0f;
    pBalls[0].zOrder = INT16_MAX;

    for (size_t i = 0; i < 360; ++i) {
        if (i >= 0 && i < 60)
            lineColor = { 1.0f, i / 59.0f, 0.0f };
        else if (i >= 60 && i < 120)
            lineColor = { 1.0f - (i - 60) / 59.0f, 1.0f, 0.0f };
        else if (i >= 120 && i < 180)
            lineColor = { 0.0f, 1.0f, (i - 120) / 59.0f };
        else if (i >= 180 && i < 240)
            lineColor = { 0.0f, 1.0f - (i - 180) / 59.0f, 1.0f };
        else if (i >= 240 && i < 300)
            lineColor = { (i - 240) / 59.0f, 0.0f, 1.0f };
        else if (i >= 300 && i < 360)
            lineColor = { 1.0f, 0.0f, 1.0f - (i - 300) / 59.0f };

        pLines.push_back(Treels::Objects::Line());
        pLines[i].zOrder = (short)i;
        pLines[i].color = lineColor;
        pLines[i].from.x = center.x + ::std::cosf(i / 180.0f * (float)M_PI) * pBalls[0].radiusX;
        pLines[i].from.y = center.y - ::std::sinf(i / 180.0f * (float)M_PI) * pBalls[0].radiusY;
    }

    //loadCircles(&pBalls);
    //loadLines(&pLines);
    //refresh();
}

void TreelsApplication::run() {
    if (reset) {
        multiplier = 0.0f;
        dephase = 0.0f;
    }
    if (inc)
        multiplier += shifting ? 0.0001f : speeding ? 1.0f : 0.01f;
    if (dec)
        multiplier -= shifting ? 0.0001f : speeding ? 1.0f : 0.01f;
    if (rol)
        dephase += shifting ? 0.01f : 1.0f;
    if (ror)
        dephase -= shifting ? 0.01f : 1.0f;

    if (multiplier > 180.0f)
        multiplier -= 360.0f;
    if (multiplier < -180.0f)
        multiplier += 360.0f;
    if (dephase > 180.0f)
        dephase -= 360.0f;
    if (dephase < -180.0f)
        dephase += 360.0f;

    float rad, deph;
    for (size_t i = 0; i < pLines.size(); ++i) {
        rad = i / 180.0f * (float)M_PI;
        deph = dephase / 180.0f * (float)M_PI;

        pLines[i].from.x = center.x + ::std::cosf(rad + deph) * pBalls[0].radiusX;
        pLines[i].from.y = center.y - ::std::sinf(rad + deph) * pBalls[0].radiusY;

        pLines[i].to.x = center.x + ::std::cosf(rad * multiplier + deph) * pBalls[0].radiusX;
        pLines[i].to.y = center.y - ::std::sinf(rad * multiplier + deph) * pBalls[0].radiusY;
    }
}

void TreelsApplication::keyPress(Treels::Action action, Treels::Key key) {
    if (key == Treels::Key::EscapeKey)
        close();
    if (key == Treels::Key::R)
        reset = action;
    if (key == Treels::Key::ArrowUp)
        inc = action;
    if (key == Treels::Key::ArrowDown)
        dec = action;
    if (key == Treels::Key::ArrowLeft)
        rol = action;
    if (key == Treels::Key::ArrowRight)
        ror = action;
    if (key == Treels::Key::Shift)
        shifting = action;
    if (key == Treels::Key::Control)
        speeding = action;
    if (key == Treels::Key::F11) {
        if (action) {
            fs = (!fs);
            //fullscreen(fs);
        }
    }
}

void TreelsApplication::resize(float newWidth, float newHeight) {
    windowWidth = newWidth;
    windowHeight = newHeight;

    center.x = windowWidth / 2.0f;
    center.y = windowHeight / 2.0f;
    pBalls[0].center = center;
    pBalls[0].radiusX = pBalls[0].radiusY = (windowWidth < windowHeight ? windowWidth : windowHeight) / 2.0f;
}

void TreelsApplication::closing() {
    //unloadAll();
}
