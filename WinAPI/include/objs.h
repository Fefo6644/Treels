#ifndef _OBJS_H_
#define _OBJS_H_

namespace objs {
    typedef struct Location
    {
        Location(int x, int y) : x(x), y(y) {}

        int x;
        int y;
    } Location;

    typedef struct Size
    {
        Size(int Width, int Height) : Width(Width), Height(Height) {}

        int Width;
        int Height;
    } Size;

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
