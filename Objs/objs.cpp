#include "stdafx.h"

#include "objs.h"

// Button
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
    m_x = x;
    m_y = y;

}

void objs::Button::SetLocation(Location location)
{
    m_x = location.x;
    m_y = location.y;

}

void objs::Button::SetSize(int Width, int Height)
{
    m_w = Width;
    m_h = Height;

}

void objs::Button::SetSize(Size size)
{
    m_w = size.Width;
    m_h = size.Height;

}
