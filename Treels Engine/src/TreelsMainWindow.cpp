#include "pch.h"

#include "Objects.h"
#include "Grapher.h"

#include "TreelsMainWindow.h"

/*---------------- wWinMain ----------------*/

TreelsMainWindow* appWin;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {

	appWin = new TreelsMainWindow();

	if (!appWin->CreateWnd(L"Application", WS_OVERLAPPEDWINDOW, hInstance))
		return -1;

	AppSetup(appWin);
	appWin->wait = false;

	ShowWindow(appWin->_hwnd, nCmdShow);

	MSG msg = { NULL };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

/*---------------- Events ----------------*/

LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_CLOSE:
		if (appWin->Closing != nullptr)
			appWin->Closing();
		break;

	case WM_CREATE:
		appWin->hasWindowLoaded = true;
		if (appWin->Load != nullptr)
			appWin->Load();
		return 0;

	case WM_SETCURSOR:
		if (appWin->hasWindowLoaded == true) {
			RECT clientArea;
			GetClientRect(hwnd, &clientArea);

			POINT cursorPos;
			GetCursorPos(&cursorPos);
			ScreenToClient(hwnd, &cursorPos);

			if (cursorPos.x < clientArea.right && cursorPos.x > 0 &&
				cursorPos.y < clientArea.bottom && cursorPos.y > 0) {
				SetCursor(appWin->cursor);
				return true;
			}
		}
		break;

	case WM_DESTROY:
		appWin->closing = true;
		appWin->_t.join();
		delete appWin->renderer;
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		appWin->renderer->Resize(lParam);
		if (appWin->Resize != nullptr)
			appWin->Resize((float)LOWORD(lParam), (float)HIWORD(lParam));
		return 0;

	case WM_KEYDOWN:
		if (appWin->KeyPress != nullptr)
			appWin->KeyPress(Action::Press, (Key)wParam);
		return 0;
	case WM_KEYUP:
		if (appWin->KeyPress != nullptr)
			appWin->KeyPress(Action::Release, (Key)wParam);
		return 0;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0 && appWin->MouseWheel != nullptr)
			appWin->MouseWheel(Action::Up, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0 && appWin->MouseWheel != nullptr)
			appWin->MouseWheel(Action::Down, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_LBUTTONDOWN:
		if (appWin->MouseLeftClick != nullptr)
			appWin->MouseLeftClick(Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		if (appWin->MouseLeftClick != nullptr)
			appWin->MouseLeftClick(Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_MBUTTONDOWN:
		if (appWin->MouseMiddleClick != nullptr)
			appWin->MouseMiddleClick(Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
		if (appWin->MouseMiddleClick != nullptr)
			appWin->MouseMiddleClick(Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_RBUTTONDOWN:
		if (appWin->MouseRightClick != nullptr)
			appWin->MouseRightClick(Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		if (appWin->MouseRightClick != nullptr)
			appWin->MouseRightClick(Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_MOUSEMOVE:
		if (appWin->MouseMove != nullptr)
			appWin->MouseMove((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*---------------- Create Window - Register Window Class ----------------*/

bool TreelsMainWindow::CreateWnd(const wchar_t* lpWindowName, DWORD dwStyle, HINSTANCE hInstance, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu) {
	RECT desiredClientArea = { x, y, nWidth, nHeight };
	AdjustWindowRect(&desiredClientArea, WS_OVERLAPPEDWINDOW, false);

	WNDCLASS wc = { NULL };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MainWindow";

	RegisterClass(&wc);

	_hwnd = CreateWindow(
		L"MainWindow", lpWindowName, dwStyle, 0, 0,
		desiredClientArea.right, desiredClientArea.bottom,
		hWndParent, hMenu, hInstance, this
	);

	if (_hwnd != nullptr) {
		std::srand((unsigned int)std::time(nullptr));
		cursor = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_arrow.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		renderer->Init();
		_t = std::thread(&TreelsMainWindow::Draw, this);
		return true;
	}

	return false;
}

/*---------------- Engine main rendering loop ----------------*/

void TreelsMainWindow::Draw() {
	while (wait == true);
	while (closing == false) {
		AppLoop();
		renderer->Draw();
	}
}

/*---------------- API Functions ----------------*/

void TreelsMainWindow::GetWindowSize(float* width, float* height) {
	RECT rc;
	GetClientRect(_hwnd, &rc);
	*width = (float)rc.right;
	*height = (float)rc.bottom;
}

float TreelsMainWindow::GetRefreshRate() {
	return renderer->GetRefreshRate();
}

void TreelsMainWindow::Close() { SendMessage(_hwnd, WM_CLOSE, NULL, NULL); }

void TreelsMainWindow::LoadCircles(std::vector<objs::Circle*>* circlesArray) { renderer->LoadCircles(circlesArray); }
void TreelsMainWindow::UnloadCircles() { renderer->UnloadCircles(); }
void TreelsMainWindow::LoadRectangles(std::vector<objs::Rectangle*>* rectanglesArray) { renderer->LoadRectangles(rectanglesArray); }
void TreelsMainWindow::UnloadRectangles() { renderer->UnloadRectangles(); }
void TreelsMainWindow::LoadLines(std::vector<objs::Line*>* linesArray) { renderer->LoadLines(linesArray); }
void TreelsMainWindow::UnloadLines() { renderer->UnloadLines(); }
void TreelsMainWindow::Unload() { renderer->Unload(); }
void TreelsMainWindow::Refresh() { renderer->Refresh(); }

void TreelsMainWindow::SetBackgroundColor(float r, float g, float b) { renderer->SetBackgroundColor(r, g, b); }
