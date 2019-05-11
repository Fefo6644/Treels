#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "basewin.h"

namespace proj {

	template <class T> void SafeRelease(T** ppT) {
		if (*ppT) {
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	class MainWindow : public BaseWindow<MainWindow> {
		ID2D1Factory7* pFactory;
		ID2D1HwndRenderTarget* pRenderTarget;

		std::atomic<BOOL>	painting = FALSE;
		std::atomic<UINT>	closing = 0;
		BOOL	hasLoaded = FALSE;
		BOOL	isKeyBeingPressedOrReleased = FALSE;
		HCURSOR niceArrow = nullptr;
		HCURSOR resizeTopBot = nullptr;
		HCURSOR resizeLeftRight = nullptr;
		HCURSOR resizeCornerNESW = nullptr;
		HCURSOR resizeCornerNWSE = nullptr;

		HRESULT	CreateGraphicsResources();
		void	DiscardGraphicsResources();
		void	OnPaint();
		void	Resize();
		int		OnCreate();

	public:

		MainWindow() : pFactory(NULL), pRenderTarget(NULL) {}

		PCWSTR	ClassName() const { return L"Window Class"; }
		LRESULT	HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

}

#endif	// _MAINWINDOW_H_
