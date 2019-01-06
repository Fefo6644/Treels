#include "stdafx.h"

#include "objs.h"

/************************** Begin Button **************************/
BOOL objs::Button::Create(HWND parentWindow, LPCWSTR text, Style style, Location location, Size size)
{
    m_hwnd = CreateWindowEx(
        NULL,
        L"BUTTON",
        text,
        WS_VISIBLE | WS_TABSTOP | WS_CHILD | style,
        location.x, location.y,
        size.Width, size.Height,
        parentWindow, NULL, (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);

    if (m_hwnd == NULL)
        return FALSE;

    m_x = location.x;
    m_y = location.y;
    m_w = size.Width;
    m_h = size.Height;

    return TRUE;
}

HWND objs::Button::GetWindowHandle()
{
    return m_hwnd;
}

objs::Location objs::Button::GetLocation()
{
    return Location(m_x, m_y);
}

objs::Size objs::Button::GetSize()
{
    return Size(m_w, m_h);
}

void objs::Button::SetLocation(int x, int y)
{
    SetWindowPos(m_hwnd, HWND_TOP, x, y, m_w, m_h, NULL);

    m_x = x;
    m_y = y;
}

void objs::Button::SetLocation(Location location)
{
    SetWindowPos(m_hwnd, HWND_TOP, location.x, location.y, m_w, m_h, NULL);

    m_x = location.x;
    m_y = location.y;
}

void objs::Button::SetSize(int Width, int Height)
{
    SetWindowPos(m_hwnd, HWND_TOP, m_x, m_y, Width, Height, NULL);

    m_w = Width;
    m_h = Height;
}

void objs::Button::SetSize(Size size)
{
    SetWindowPos(m_hwnd, HWND_TOP, m_x, m_y, size.Width, size.Height, NULL);

    m_w = size.Width;
    m_h = size.Height;
}
/************************** End Button **************************/

/************************** Begin Slider **************************/

BOOL objs::Slider::Create(HWND parentWindow, Location location, Size size, UINT16 maxValue, BOOL autoTicks)
{
    m_hwnd = CreateWindowEx(
        NULL,
        TRACKBAR_CLASS,
        L"Trackbar Control",
        WS_CHILD | WS_VISIBLE | TBS_TRANSPARENTBKGND | (TBS_AUTOTICKS & autoTicks),
        location.x, location.y,
        size.Width, size.Height,
        parentWindow, NULL, (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);

    if (m_hwnd == NULL)
        return FALSE;

    SendMessage(m_hwnd, TBM_SETRANGE,
        (WPARAM)TRUE,
        (LPARAM)MAKELONG(0, maxValue));

    SendMessage(m_hwnd, TBM_SETPAGESIZE,
        0, (LPARAM)4);

    m_x = location.x;
    m_y = location.y;
    m_w = size.Width;
    m_h = size.Height;

    return TRUE;
}

HWND objs::Slider::GetWindowHandle()
{
    return m_hwnd;
}

objs::Location objs::Slider::GetLocation()
{
    return Location(m_x, m_y);
}

objs::Size objs::Slider::GetSize()
{
    return Size(m_w, m_h);
}

void objs::Slider::SetLocation(int x, int y)
{
    SetWindowPos(m_hwnd, HWND_TOP, x, y, m_w, m_h, NULL);

    m_x = x;
    m_y = y;
}

void objs::Slider::SetLocation(Location location)
{
    SetWindowPos(m_hwnd, HWND_TOP, location.x, location.y, m_w, m_h, NULL);

    m_x = location.x;
    m_y = location.y;
}

void objs::Slider::SetSize(int Width, int Height)
{
    SetWindowPos(m_hwnd, HWND_TOP, m_x, m_y, Width, Height, NULL);

    m_w = Width;
    m_h = Height;
}

void objs::Slider::SetSize(Size size)
{
    SetWindowPos(m_hwnd, HWND_TOP, m_x, m_y, size.Width, size.Height, NULL);

    m_w = size.Width;
    m_h = size.Height;
}
/************************** End Slider **************************/
