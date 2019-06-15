#include "pch.h"

#include "Controls.h"

namespace ctrls {

	void Controls::_controlsMain() {
		checkingControls = true;
		while (true) {
			std::unique_lock<std::mutex> lk(m);
			keysStateChanged.wait(lk);

			if ((keysState & KEY_CLOSE_DOWN) && *closing == 0) {
				*closing = 1;
				break;
			}
		}
		checkingControls = false;
	}

	int Controls::Init(HWND* hWnd, std::atomic<unsigned int>* closing) {
		try {
			if (checkingControls == false) {
				_t = std::thread(&Controls::_controlsMain, this);
				_t.detach();
				this->closing = closing;
				this->hWnd = hWnd;
				return 1;
			}
			else
				return 2;
		}
		catch (const std::system_error * se) {
			MessageBox(*hWnd, L"An error has ocurred. Controls could not be initialized. Program will shut down.", L"System Error", MB_OK);

			wchar_t* seWhat = new wchar_t[strlen(se->what()) + 1], *seCode = new wchar_t[strlen(se->code().message().c_str()) + 1];
			mbstowcs_s(NULL, seWhat, strlen(se->what()) + 1, se->what(), strlen(se->what()));
			mbstowcs_s(NULL, seCode, strlen(se->code().message().c_str()) + 1, se->code().message().c_str(), strlen(se->code().message().c_str()));

			MessageBox(*hWnd, (const wchar_t*)seWhat, (const wchar_t*)seCode, MB_OK);

			delete[] seWhat;
			delete[] seCode;

			return 0;
		}
	}

	void Controls::ParseKeyEvent(WPARAM wParam, bool isKeyBeingPressedOrReleased) {
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
