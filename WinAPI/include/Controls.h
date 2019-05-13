#ifndef _CONTROLS_H_
#define _CONTROLS_H_

namespace proj {

	constexpr bool PRESSED = true;
	constexpr bool RELEASED = false;

	constexpr word KEY_CLOSE_DOWN = 0b1000000000000000;

	constexpr word KEY_CLOSE_UP = 0b0111111111111111;

	class Controls {

	public:

		int ControlsMain(HWND* hWnd, std::atomic<unsigned int>* closing);
		void ParseKeyEvent(WPARAM keyEvent, bool isKeyBeingPressedOrReleased);

	private:
		std::atomic<word> keysState = 0b0000000000000000;
		std::atomic<bool> checkingControls = false;
		std::atomic<unsigned int>* closing = nullptr;

		std::condition_variable keysStateChanged = std::condition_variable();
		std::mutex m = std::mutex();

		HWND* hWnd = nullptr;

		std::thread _t = std::thread();
		void _controlsMain();

	};

}

#endif
