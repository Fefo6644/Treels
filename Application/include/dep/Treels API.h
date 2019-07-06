#ifndef _TREELS_API_H_
#define _TREELS_API_H_

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Treels.lib")

#ifndef _WIN64
#pragma comment(linker, "/include:_wWinMain@16")
#else
#pragma comment(linker, "/include:wWinMain")
#endif // !_WIN64

#include <Windows.h>
#include <d2d1_3.h>
#include <vector>

#include "Objects.h"

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

class TreelsMainWindow {
public:
	void LoadCircles(std::vector<objs::Circle*>* circlesArray);
	void UnloadCircles();
	void LoadRectangles(std::vector<objs::Rectangle*>* rectanglesArray);
	void UnloadRectangles();
	void LoadLines(std::vector<objs::Line*>* linesArray);
	void UnloadLines();
	void Unload();
	void Refresh();

	void SetBackgroundColor(float r, float g, float b);
	void GetWindowSize(float* width, float* height);
	float GetRefreshRate();

	void Close();

	/*---------------- Mouse input events ----------------*/

	void (*MouseMove)(float x, float y) = nullptr;
	void (*MouseWheel)(Action action, float x, float y) = nullptr;
	void (*MouseLeftClick)(Action action, float x, float y) = nullptr;
	void (*MouseMiddleClick)(Action action, float x, float y) = nullptr;
	void (*MouseRightClick)(Action action, float x, float y) = nullptr;

	/*---------------- Keyboard input events ----------------*/

	void (*KeyPress)(Action action, Key key) = nullptr;

	/*---------------- Other events ----------------*/

	void (*Resize)(float newWidth, float newHeight) = nullptr;
	void (*Load)() = nullptr;
	void (*Closing)() = nullptr;
};

#endif	// !_TREELS_API_H_
