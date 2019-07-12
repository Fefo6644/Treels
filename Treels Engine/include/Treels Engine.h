#ifndef _TREELSENGINE_H_
#define _TREELSENGINE_H_

struct TreelsApplication;

namespace Treels {
	/*---------------- Enumerators (to be passed arguments to external event-triggered functions) ----------------*/

	enum Action : bool {
		Press = true,
		Release = false,
		Up = true,
		Down = false
	};

	enum Key : char {
		Backspace = 0x08,
		Tab = 0x09,
		Enter = 0x0D,
		Shift = 0x10,
		Control = 0x11,
		EscapeKey = 0x1B,
		Space = ' ',

		UpArrow = 0x26,
		DownArrow = 0x28,
		LeftArrow = 0x25,
		RightArrow = 0x27,

		Number0 = '0',
		Number1 = '1',
		Number2 = '2',
		Number3 = '3',
		Number4 = '4',
		Number5 = '5',
		Number6 = '6',
		Number7 = '7',
		Number8 = '8',
		Number9 = '9',

		Numpad0 = '0' + '0',
		Numpad1 = '1' + '0',
		Numpad2 = '2' + '0',
		Numpad3 = '3' + '0',
		Numpad4 = '4' + '0',
		Numpad5 = '5' + '0',
		Numpad6 = '6' + '0',
		Numpad7 = '7' + '0',
		Numpad8 = '8' + '0',
		Numpad9 = '9' + '0',

		NumpadMultiply = 0x6A,
		NumpadAdd = 0x6B,
		NumpadSubstract = 0x6D,
		NumpadDecimal = 0x6E,
		NumpadDivide = 0x6F,

		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z'
	};

	struct TreelsEngine {
		bool CreateWnd(const wchar_t* lpWindowName,
			DWORD dwStyle,
			HINSTANCE hInstance = GetModuleHandle(NULL),
			int x = 0,
			int y = 0,
			int nWidth = 400,
			int nHeight = 400,
			HWND hWndParent = 0,
			HMENU hMenu = 0);

		void LoadCircles(::std::vector<Objects::Circle>* circlesArray);
		void UnloadCircles();
		void LoadRectangles(::std::vector<Objects::Rectangle>* rectanglesArray);
		void UnloadRectangles();
		void LoadLines(::std::vector<Objects::Line>* linesArray);
		void UnloadLines();
		void Unload();
		void Refresh();

		void SetBackgroundColor(float r, float g, float b);
		void GetWindowSize(float& width, float& height);
		float GetRefreshRate();

		void Close();

		/*---------------- Mouse input events ----------------*/

		void (TreelsApplication::* MouseMove)(float x, float y) = nullptr;
		void (TreelsApplication::* MouseWheel)(Action action, float x, float y) = nullptr;
		void (TreelsApplication::* MouseLeftClick)(Action action, float x, float y) = nullptr;
		void (TreelsApplication::* MouseMiddleClick)(Action action, float x, float y) = nullptr;
		void (TreelsApplication::* MouseRightClick)(Action action, float x, float y) = nullptr;

		/*---------------- Keyboard input events ----------------*/

		void (TreelsApplication::* KeyPress)(Action action, Key key) = nullptr;

		/*---------------- Other events ----------------*/

		void (TreelsApplication::* Resize)(float newWidth, float newHeight) = nullptr;
		void (TreelsApplication::* Load)() = nullptr;
		void (TreelsApplication::* Closing)() = nullptr;

		/*---------------- Other members----------------*/
		bool hasWindowLoaded = false;
		HCURSOR cursor = nullptr;
		HWND _hwnd = nullptr;
		Grapher::Grapher* renderer = nullptr;
		TreelsApplication* app = nullptr;

		void Draw();
	};
}
LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);

/*---------------- Application side ----------------*/

struct TreelsApplication {
	TreelsApplication(Treels::TreelsEngine*);
	void TreelsRun();
	static size_t GetStructSize();
};

#endif	// !_TREELSENGINE_H_
