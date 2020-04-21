#ifndef _TREELSAPI_H_
#define _TREELSAPI_H_

#include "includes.h"

namespace Treels {
    /*---------------- Enumerators (to be passed arguments to external event-triggered functions) ----------------*/

    enum Action : bool {
        Press = true,
        Release = false,
        Up = true,
        Down = false
    };

    enum Key : unsigned short {
        Backspace = 0x08,
        Tab = 0x09,
        Enter = 0x0D,
        Shift = 0x10,
        Control = 0x11,
        EscapeKey = 0x1B,
        Space = ' ',

        ArrowLeft = 0x25,
        ArrowUp,
        ArrowRight,
        ArrowDown,

        Number0 = '0',
        Number1,
        Number2,
        Number3,
        Number4,
        Number5,
        Number6,
        Number7,
        Number8,
        Number9,

        A = 'A',
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Numpad0 = '0' + '0',
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,

        NumpadMultiply,
        NumpadAdd,
        NumpadSeparator,
        NumpadSubstract,
        NumpadDecimal,
        NumpadDivide,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24
    };

    struct TreelsEngine {
        /* Window creation and registration */

        bool createWnd(const wchar_t* lpWindowName,
            DWORD dwStyle,
            HINSTANCE hInstance = GetModuleHandle(NULL),
            int x = 0,
            int y = 0,
            int nWidth = 400,
            int nHeight = 400,
            HWND hWndParent = 0,
            HMENU hMenu = 0);

        /*---------------- API Functions ----------------*/

        void getWindowSize(float& width, float& height);
        void setWindowTitle(::std::wstring title);
        float getRefreshRate();

        void close();

        /*---------------- Mouse input events ----------------*/

        virtual void mouseMove(float x, float y) {};
        virtual void mouseWheel(Action action, float x, float y) {};
        virtual void mouseLeftClick(Action action, float x, float y) {};
        virtual void mouseMiddleClick(Action action, float x, float y) {};
        virtual void mouseRightClick(Action action, float x, float y) {};

        /*---------------- Keyboard input events ----------------*/

        virtual void keyPress(Action action, Key key) {};

        /*---------------- Other events ----------------*/

        virtual void resize(float newWidth, float newHeight) {};
        virtual void load() {};
        virtual void closing() {};

        /*---------------- Mandatory member functions ----------------*/

        virtual void setup() = 0;
        virtual void run() = 0;

    private:
        /*---------------- Other members----------------*/

        bool hasWindowLoaded = false;
        HWND hwnd = nullptr;
        ATOM registeredClass;
        ::std::unique_ptr<Grapher::Grapher> renderer;

        void loop();

        HCURSOR cursor;
    };
}
/* Standard WindowProc so Windows can tell the engine [what's going on](https://www.youtube.com/watch?v=6NXnxTNIWkc) */

LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);

#endif	// !_TREELSAPI_H_
