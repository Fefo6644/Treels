#include "pch.h"

#include "Controls.h"

namespace proj {

	std::atomic<WORD> Controls::keysState = 0b0000000000000000;
	std::atomic<BOOL> Controls::checkingControls = FALSE;
	std::atomic<UINT>* Controls::closing = nullptr;

	std::condition_variable Controls::keysStateChanged = std::condition_variable();
	std::mutex Controls::m = std::mutex();

	HWND* Controls::hWnd = nullptr;

	std::thread Controls::_t = std::thread();

	void Controls::_controlsMain() {
		checkingControls = TRUE;
		while (true) {
			std::unique_lock<std::mutex> lk(m);
			keysStateChanged.wait(lk);

			if ((keysState & KEY_CLOSE_DOWN) && *closing == 0) {
				*closing = 1;
				break;
			}
		}
		checkingControls = FALSE;
	}

	int Controls::ControlsMain(HWND* hWnd, std::atomic<UINT>* closing) {
		try {
			if (checkingControls == FALSE) {
				_t = std::thread(&Controls::_controlsMain);
				_t.detach();
				Controls::closing = closing;
				Controls::hWnd = hWnd;
				return 1;
			}
			else
				return 2;
		}
		catch (const std::system_error * se) {
			MessageBox(*hWnd, L"An error has ocurred.\r\n\
Controls could not be initialized.\r\n\
Program will shut down.", L"System Error", MB_OK);

			WCHAR* seWhat = new WCHAR[strlen(se->what()) + 1], *seCode = new WCHAR[strlen(se->code().message().c_str()) + 1];
			mbstowcs_s(NULL, seWhat, strlen(se->what()) + 1, se->what(), strlen(se->what()));
			mbstowcs_s(NULL, seCode, strlen(se->code().message().c_str()) + 1, se->code().message().c_str(), strlen(se->code().message().c_str()));

			MessageBox(*hWnd, (LPCWSTR)seWhat, (LPCWSTR)seCode, MB_OK);

			delete[] seWhat;
			delete[] seCode;

			return 0;
		}
	}

	void Controls::ParseKeyEvent(WPARAM wParam, BOOL isKeyBeingPressedOrReleased) {
		if (isKeyBeingPressedOrReleased == PRESSED) {
			if (wParam == VK_ESCAPE)
				keysState |= KEY_CLOSE_DOWN;
		}
		else {
			if (wParam == VK_ESCAPE)
				keysState &= KEY_CLOSE_UP;
		}

		keysStateChanged.notify_one();
	}

}
