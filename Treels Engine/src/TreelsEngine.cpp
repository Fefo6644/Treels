#include "pch.h"

#include "Grapher.h"

#include "TreelsEngine.h"

/*---------------- wWinMain ----------------*/

::std::unique_ptr<Treels::TreelsEngine> treels;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
    treels = ::std::make_unique<Treels::TreelsEngine>();

    if (!treels->createWnd(L"TreelsApp", WS_OVERLAPPEDWINDOW, hInstance))
        return -1;

    ShowWindow(treels->hwnd, nCmdShow);

    MSG msg = { NULL };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message != WM_SIZE)
            treels->Draw();
    }

    return 0;
}

/*---------------- Events ----------------*/
LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            /* Close event. */
            treels->closing();
            break;

        case WM_CREATE:
            /* Load event and creation signal to enable setting the cursor. */
            treels->hasWindowLoaded = true;
            treels->load();
            return 0;

        case WM_SETCURSOR:
            /*
            · If the window has been created,
                only apply that mouse thing if the mouse is in the client area.
            */
            if (treels->hasWindowLoaded == true) {
                RECT clientArea;
                GetClientRect(hwnd, &clientArea);

                POINT cursorPos;
                GetCursorPos(&cursorPos);
                ScreenToClient(hwnd, &cursorPos);

                if (cursorPos.x < clientArea.right && cursorPos.x > 0 &&
                    cursorPos.y < clientArea.bottom && cursorPos.y > 0) {
                    SetCursor(treels->cursor);
                    return true;
                }
            }
            break;

        case WM_DESTROY:
            //delete treels->renderer;
            //treels->renderer = nullptr;
            //operator delete(treels->app, TreelsApplication::GetStructSize());
            //treels->app = nullptr;
            PostQuitMessage(0);
            return 0;


            /*******************************************************************************/
            /********************** EVENT SYSTEM CODE SECTION WARNING **********************/
            /*********** RESIZING - KEY STROKES - MOUSE MOVEMENT - MOUSE CLICKS ************/
            /*******************************************************************************/

        case WM_SIZE:
            //appWin->renderer->Resize(lParam);
            treels->resize((float)LOWORD(lParam), (float)HIWORD(lParam));
            return 0;

        case WM_KEYDOWN:
            treels->keyPress(Treels::Action::Press, (Treels::Key)wParam);
            return 0;
        case WM_KEYUP:
            treels->keyPress(Treels::Action::Release, (Treels::Key)wParam);
            return 0;

        case WM_MOUSEWHEEL:
            if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
                treels->mouseWheel(Treels::Action::Up, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
                treels->mouseWheel(Treels::Action::Down, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;

        case WM_LBUTTONDOWN:
            treels->mouseLeftClick(Treels::Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;
        case WM_LBUTTONUP:
            treels->mouseLeftClick(Treels::Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;

        case WM_MBUTTONDOWN:
            treels->mouseMiddleClick(Treels::Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;
        case WM_MBUTTONUP:
            treels->mouseMiddleClick(Treels::Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;

        case WM_RBUTTONDOWN:
            treels->mouseRightClick(Treels::Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;
        case WM_RBUTTONUP:
            treels->mouseRightClick(Treels::Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;

        case WM_MOUSEMOVE:
            treels->mouseMove((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            return 0;

        default:
            /* If none of those events took place, let Windows handle it... */
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

namespace Treels {
    /*---------------- Create Window - Register Window Class ----------------*/
    bool TreelsEngine::createWnd(const wchar_t* lpWindowName, DWORD dwStyle, HINSTANCE hInstance, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu) {
        /* Adjust the area of the window so the requested area is the client area */
        RECT desiredClientArea = { x, y, nWidth, nHeight };
        AdjustWindowRect(&desiredClientArea, WS_OVERLAPPEDWINDOW, false);

        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"MainWindow";

        registeredClass = RegisterClass(&wc);
        hwnd = CreateWindow(
            L"MainWindow", lpWindowName, dwStyle, 0, 0,
            desiredClientArea.right - desiredClientArea.left, desiredClientArea.bottom - desiredClientArea.top,
            hWndParent, hMenu, hInstance, NULL);

        if (hwnd != NULL) {
            /*
            · If the window creation is successful, load the default Aero arrow cursor in client area.
            */
            cursor = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_arrow.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            renderer = ::std::make_unique<Grapher::Grapher>(hwnd);
            renderer->init();

            /*
            · Ask for the struct size because the engine doesn't really know it
                (as far as it knows, it's just one function);
            · Allocate that amount in the heap because it might exceed the stack size;
            · Then try and allocate an actual application instance in that memory;
            · If it fails... it fails.
            */
            //app = (TreelsApplication*)operator new(TreelsApplication::GetStructSize());
            //try {
            //	app = new(app) TreelsApplication(this);
            //}
            //catch (...) {
            //	operator delete(app, TreelsApplication::GetStructSize());
            //	throw ::std::bad_alloc{};
            //}
            return true;
        }
        else {
            ::std::wstringstream err;
            err << L"Graphics engine could not initialize the window. Error code: " << ::std::hex << GetLastError();
            MessageBox(NULL, err.str().c_str(), NULL, MB_ICONERROR);
            return false;
        }
    }

    /*---------------- Engine main rendering loop ----------------*/

    void TreelsEngine::loop() {
        if (renderer != nullptr) {
            run();
            //renderer->Draw();
        }
    }

    /*---------------- API Functions ----------------*/

    void TreelsEngine::getWindowSize(float& width, float& height) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        width = (float)rc.right - rc.left;
        height = (float)rc.bottom - rc.top;
    }

    void TreelsEngine::close() { SendMessage(hwnd, WM_CLOSE, NULL, NULL); }

    void TreelsEngine::setWindowTitle(::std::wstring title) { SetWindowText(hwnd, title.c_str()); }
    float TreelsEngine::getRefreshRate() { return renderer->getRefreshRate(); }
}
