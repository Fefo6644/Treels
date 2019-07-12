#include "pch.h"

#include "Objects.h"
#include "Grapher.h"

#include "Treels Engine.h"

/*---------------- wWinMain ----------------*/

Treels::TreelsEngine* appWin = nullptr;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	appWin = new Treels::TreelsEngine;

	if (!appWin->CreateWnd(L"TreelsApp", WS_OVERLAPPEDWINDOW, hInstance))
		return -1;

	ShowWindow(appWin->_hwnd, nCmdShow);

	MSG msg = { NULL };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message != WM_SIZE)
			appWin->Draw();
	}

	delete appWin;
	return 0;
}

/*---------------- Events ----------------*/

LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_CLOSE:
		if (appWin->Closing != nullptr)
			(appWin->app->*(appWin->Closing))();
		break;

	case WM_CREATE:
		appWin->hasWindowLoaded = true;
		if (appWin->Load != nullptr)
			(appWin->app->*(appWin->Load))();
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
		delete appWin->renderer;
		appWin->renderer = nullptr;
		operator delete(appWin->app, TreelsApplication::GetStructSize());
		appWin->app = nullptr;
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		appWin->renderer->Resize(lParam);
		if (appWin->Resize != nullptr)
			(appWin->app->*(appWin->Resize))((float)LOWORD(lParam), (float)HIWORD(lParam));
		return 0;

	case WM_KEYDOWN:
		if (appWin->KeyPress != nullptr)
			(appWin->app->*(appWin->KeyPress))(Treels::Action::Press, (Treels::Key)wParam);
		return 0;
	case WM_KEYUP:
		if (appWin->KeyPress != nullptr)
			(appWin->app->*(appWin->KeyPress))(Treels::Action::Release, (Treels::Key)wParam);
		return 0;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0 && appWin->MouseWheel != nullptr)
			(appWin->app->*(appWin->MouseWheel))(Treels::Action::Up, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0 && appWin->MouseWheel != nullptr)
			(appWin->app->*(appWin->MouseWheel))(Treels::Action::Down, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_LBUTTONDOWN:
		if (appWin->MouseLeftClick != nullptr)
			(appWin->app->*(appWin->MouseLeftClick))(Treels::Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		if (appWin->MouseLeftClick != nullptr)
			(appWin->app->*(appWin->MouseLeftClick))(Treels::Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_MBUTTONDOWN:
		if (appWin->MouseMiddleClick != nullptr)
			(appWin->app->*(appWin->MouseMiddleClick))(Treels::Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
		if (appWin->MouseMiddleClick != nullptr)
			(appWin->app->*(appWin->MouseMiddleClick))(Treels::Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_RBUTTONDOWN:
		if (appWin->MouseRightClick != nullptr)
			(appWin->app->*(appWin->MouseRightClick))(Treels::Action::Press, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		if (appWin->MouseRightClick != nullptr)
			(appWin->app->*(appWin->MouseRightClick))(Treels::Action::Release, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;

	case WM_MOUSEMOVE:
		if (appWin->MouseMove != nullptr)
			(appWin->app->*(appWin->MouseMove))((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

namespace Treels {
	/*---------------- Create Window - Register Window Class ----------------*/

	bool TreelsEngine::CreateWnd(const wchar_t* lpWindowName, DWORD dwStyle, HINSTANCE hInstance, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu) {
		RECT desiredClientArea = { x, y, nWidth, nHeight };
		AdjustWindowRect(&desiredClientArea, WS_OVERLAPPEDWINDOW, false);

		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = L"MainWindow";

		RegisterClass(&wc);

		_hwnd = CreateWindow(
			L"MainWindow", lpWindowName, dwStyle, 0, 0,
			desiredClientArea.right - desiredClientArea.left, desiredClientArea.bottom - desiredClientArea.top,
			hWndParent, hMenu, hInstance, this
		);

		if (_hwnd != nullptr) {
			cursor = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_arrow.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			renderer = new Grapher::Grapher(_hwnd, Objects::Color(0.0f, 0.0f, 0.0f, 1.0f));
			renderer->Init();

			app = (TreelsApplication*)operator new(TreelsApplication::GetStructSize());
			try {
				app = new(app) TreelsApplication(this);
			}
			catch (...) {
				operator delete(app, TreelsApplication::GetStructSize());
				throw std::bad_alloc{};
			}
			return true;
		}

		return false;
	}

	/*---------------- Engine main rendering loop ----------------*/

	void TreelsEngine::Draw() {
		if (renderer != nullptr && app != nullptr) {
			app->TreelsRun();
			renderer->Draw();
		}
	}

	/*---------------- API Functions ----------------*/

	void TreelsEngine::GetWindowSize(float& width, float& height) {
		RECT rc;
		GetClientRect(_hwnd, &rc);
		width = (float)rc.right - rc.left;
		height = (float)rc.bottom - rc.top;
	}

	float TreelsEngine::GetRefreshRate() {
		return renderer->GetRefreshRate();
	}

	void TreelsEngine::Close() { SendMessage(_hwnd, WM_CLOSE, NULL, NULL); }

	void TreelsEngine::LoadCircles(::std::vector<Objects::Circle>* circlesArray) { renderer->LoadCircles(circlesArray); }
	void TreelsEngine::UnloadCircles() { renderer->UnloadCircles(); }
	void TreelsEngine::LoadRectangles(::std::vector<Objects::Rectangle>* rectanglesArray) { renderer->LoadRectangles(rectanglesArray); }
	void TreelsEngine::UnloadRectangles() { renderer->UnloadRectangles(); }
	void TreelsEngine::LoadLines(::std::vector<Objects::Line>* linesArray) { renderer->LoadLines(linesArray); }
	void TreelsEngine::UnloadLines() { renderer->UnloadLines(); }
	void TreelsEngine::Unload() { renderer->Unload(); }
	void TreelsEngine::Refresh() { renderer->Refresh(); }

	void TreelsEngine::SetBackgroundColor(float r, float g, float b) { renderer->SetBackgroundColor(r, g, b); }
}
