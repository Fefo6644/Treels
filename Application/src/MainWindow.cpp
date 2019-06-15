#include <Windows.h>
#include <d2d1_3.h>

#include <cstdlib>
#include <ctime>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "dep/Objects.h"
#include "dep/Grapher.h"

#include "MainWindow.h"

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
	if (closing == 1) {
		closing = 2;

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

			if (cursorPos.x < clientArea.right && cursorPos.x > 0 &&
				cursorPos.y < clientArea.bottom && cursorPos.y > 0) {
				SetCursor(niceArrow);
				return true;
			}
		}
		break;

	case WM_CREATE:
		return OnCreate();

	case WM_DESTROY:
		renderer.~Grapher();

		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hasWindowLoaded = true;
		return 0;

	case WM_SIZE:
		renderer.Resize(lParam);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE && closing == 0)
			closing = 1;
		break;
	case WM_KEYUP:
		break;
	}


	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int MainWindow::OnCreate() {
	std::srand((unsigned int)std::time(nullptr));

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
		L"MainWindow", lpWindowName, dwStyle, 0, 0,
		desiredClientArea.right, desiredClientArea.bottom,
		hWndParent, hMenu, hInstance, this
	);

	renderer.Init();

	return (m_hwnd ? true : false);
}

void MainWindow::Update() { renderer.DrawObjects(nullptr); }

HWND MainWindow::Window() { return m_hwnd; }

HWND MainWindow::m_hwnd = nullptr;

grph::Grapher MainWindow::renderer = grph::Grapher(&m_hwnd, objs::Color(0.9f, 0.9f, 0.9f));

unsigned int MainWindow::closing = 0;
bool MainWindow::hasWindowLoaded = false;
HCURSOR MainWindow::niceArrow = nullptr;

/*---------------- wWinMain ----------------*/

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	MainWindow win;

	if (!win.CreateWnd(L"D2D Test", WS_OVERLAPPEDWINDOW, hInstance))
		return 0;

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			win.Update();
		}
	}

	return 0;
}