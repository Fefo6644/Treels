#ifndef _CONTROLS_H_
#define _CONTROLS_H_

namespace proj {

	constexpr BOOL PRESSED = TRUE;
	constexpr BOOL RELEASED = FALSE;

	constexpr WORD KEY_CLOSE_DOWN = 0b1000000000000000;

	constexpr WORD KEY_CLOSE_UP = 0b0111111111111111;

	class Controls {

	public:
		static int ControlsMain(HWND* hWnd, std::atomic<UINT>* closing);
		static void ParseKeyEvent(WPARAM keyEvent, BOOL isKeyBeingPressedOrReleased);

	private:
		static std::atomic<WORD> keysState;
		static std::atomic<BOOL> checkingControls;
		static std::atomic<UINT>* closing;

		static std::condition_variable keysStateChanged;
		static std::mutex m;

		static HWND* hWnd;

		static std::thread _t;
		static void _controlsMain();

	};

}

#endif
