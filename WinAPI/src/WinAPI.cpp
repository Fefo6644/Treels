#include "pch.h"

#include "Controls.h"
#include "Grapher.h"
#include "MainWindow.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	proj::MainWindow win;

	if (!win.CreateWnd(L"D2D Test", WS_OVERLAPPEDWINDOW, hInstance))
		return 0;

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
