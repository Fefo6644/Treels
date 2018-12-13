#ifndef _OBJS_H_
#define _OBJS_H_

namespace objs {
    struct Location
    {
    protected:
        int _x;
        int _y;

    public:
        Location(int x, int y) : _x(x), _y(y) {}

        int GetX();
        int GetY();
        void SetX(int x);
        void SetY(int y);
    };

    struct Size
    {
    protected:
        int _w;
        int _h;

    public:
        Size(int Width, int Height) : _w(Width), _h(Height) {}

        int GetWidth();
        int GetHeight();
        void SetWidth(int Width);
        void SetHeight(int Height);
    };

    class Button
    {
    public:

        enum Style : DWORD
        {
            ClassicButton = BS_DEFPUSHBUTTON,
            CheckBox = BS_AUTOCHECKBOX,
            Radio = BS_AUTORADIOBUTTON
        };

        BOOL Create(HWND parentWindow, LPCWSTR text = L"Hello, World!", Style style = ClassicButton, Location location = Location(10, 10), Size size = Size(75, 25));

        Location    GetLocation();
        Size        GetSize();
        HWND        GetWindowHandle();

        void        SetLocation(int x, int y);
        void        SetLocation(Location location);
        void        SetSize(int Width, int Height);
        void        SetSize(Size size);

        Button() : m_x(NULL), m_y(NULL), m_w(NULL), m_h(NULL), m_hwnd(NULL) {}

    protected:
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        HWND m_hwnd;
    };
}

#endif //_OBJS_H_
