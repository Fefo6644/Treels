#include "stdafx.h"

#include "objs.h"


// Location
int objs::Location::GetX()
{
    return _x;
}

int objs::Location::GetY()
{
    return _y;
}

void objs::Location::SetX(int x)
{
    _x = x;
}

void objs::Location::SetY(int y)
{
    _y = y;
}


// Size
int objs::Size::GetWidth()
{
    return _w;
}

int objs::Size::GetHeight()
{
    return _h;
}

void objs::Size::SetWidth(int Width)
{
    _w = Width;
}

void objs::Size::SetHeight(int Height)
{
    _h = Height;
}


// Button
BOOL objs::Button::Create(HWND parentWindow, LPCWSTR text, Style style, Location location, Size size)
{
    m_hwnd = CreateWindowEx(
        NULL,
        L"BUTTON",
        text,
        WS_VISIBLE | WS_TABSTOP | WS_CHILD | style,
        location.GetX(), location.GetY(),
        size.GetWidth(), size.GetHeight(),
        parentWindow, NULL, (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE), NULL);

    if (m_hwnd == NULL)
        return FALSE;

    m_x = location.GetX();
    m_y = location.GetY();
    m_w = size.GetWidth();
    m_h = size.GetHeight();

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
    m_x = x;
    m_y = y;

}

void objs::Button::SetLocation(Location location)
{
    m_x = location.GetX();
    m_y = location.GetY();

}

void objs::Button::SetSize(int Width, int Height)
{
    m_w = Width;
    m_h = Height;

}

void objs::Button::SetSize(Size size)
{
    m_w = size.GetWidth();
    m_h = size.GetHeight();

}
