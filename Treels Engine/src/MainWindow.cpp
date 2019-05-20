#include "pch.h"

#include "Controls.h"
#include "Grapher.h"
#include "MainWindow.h"

namespace proj {

	LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
		if (closing == 1) {
			closing = 2;

			controlsHandler.~Controls();
			renderer.~Grapher();

			PostQuitMessage(0);
			return 0;
		}

		switch (uMsg) {

		case WM_SETCURSOR:
			if (hasWindowLoaded == true) {
				RECT clientArea;
				GetClientRect(hwnd, &clientArea);

				POINT cursorPos;
				GetCursorPos(&cursorPos);
				ScreenToClient(hwnd, &cursorPos);

				if (cursorPos.x < clientArea.right && cursorPos.x > 0 && cursorPos.y < clientArea.bottom && cursorPos.y > 0) {
					SetCursor(niceArrow);
					return true;
				}
			}
			break;

		case WM_CREATE:
			return OnCreate();

		case WM_DESTROY:
			controlsHandler.~Controls();
			renderer.~Grapher();

			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			renderer.DrawVertices(nullptr);

			hasWindowLoaded = true;
			return 0;

		case WM_SIZE:
			renderer.Resize(lParam);
			return 0;

		case WM_KEYDOWN:
			isKeyBeingPressedOrReleased = PRESSED;
		case WM_KEYUP:
			controlsHandler.ParseKeyEvent(wParam, isKeyBeingPressedOrReleased);
			isKeyBeingPressedOrReleased = RELEASED;
			break;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	int MainWindow::OnCreate() {
		std::srand((unsigned int)std::time(nullptr));

		renderer.Init();

		if (controlsHandler.Init(&m_hwnd, &closing) == 0)
			return -1;

		niceArrow = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_arrow.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);

		return 0;
	}

	bool MainWindow::CreateWnd(const wchar_t* lpWindowName, DWORD dwStyle, HINSTANCE hInstance, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu) {

		RECT desiredClientArea = { x, y, nWidth, nHeight };
		AdjustWindowRect(&desiredClientArea, WS_OVERLAPPEDWINDOW, false);

		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = MainWindow::WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = L"MainWindow";

		RegisterClass(&wc);

		m_hwnd = CreateWindow(
			L"MainWindow", lpWindowName, dwStyle, 0, 0, desiredClientArea.right, desiredClientArea.bottom, hWndParent, hMenu, hInstance, this
		);

		return (m_hwnd ? true : false);
	}

	HWND MainWindow::Window() {
		return m_hwnd;
	}

	HWND MainWindow::m_hwnd = nullptr;

	Grapher MainWindow::renderer = Grapher(&m_hwnd);
	Controls MainWindow::controlsHandler = Controls();

	std::atomic<unsigned int> MainWindow::closing = 0;
	bool MainWindow::hasWindowLoaded = false;
	bool MainWindow::isKeyBeingPressedOrReleased = false;
	HCURSOR MainWindow::niceArrow = nullptr;
}
