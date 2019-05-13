#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

namespace proj {

	class MainWindow {
	private:
		static HWND m_hwnd;

		static Grapher renderer;
		static Controls controlsHandler;

		static std::atomic<unsigned int> closing;
		static bool hasWindowLoaded;
		static bool isKeyBeingPressedOrReleased;
		static HCURSOR niceArrow;

		static int OnCreate();

	public:
		static HWND Window();

		bool CreateWnd(const wchar_t* lpWindowName,
			DWORD dwStyle,
			HINSTANCE hInstance = GetModuleHandle(NULL),
			int x = 0,
			int y = 0,
			int nWidth = 400,
			int nHeight = 400,
			HWND hWndParent = 0,
			HMENU hMenu = 0);

		static LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);
	};

}

#endif	// _MAINWINDOW_H_
