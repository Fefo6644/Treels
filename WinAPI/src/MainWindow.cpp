#include "pch.h"

#include "Controls.h"
#include "Grapher.h"
#include "MainWindow.h"

namespace proj {

	HRESULT MainWindow::CreateGraphicsResources() {
		HRESULT hr = S_OK;
		if (pRenderTarget == NULL) {
			RECT rc;
			GetClientRect(m_hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hwnd, size),
				&pRenderTarget);

			if (SUCCEEDED(hr)) {
				// Call grapher. Send vertices.
			}
		}
		return hr;
	}

	void MainWindow::DiscardGraphicsResources() {
		SafeRelease(&pRenderTarget);
		SafeRelease(&pFactory);
	}

	void MainWindow::OnPaint() {
		painting = TRUE;

		HRESULT hr = CreateGraphicsResources();
		if (SUCCEEDED(hr)) {
			PAINTSTRUCT ps;
			BeginPaint(m_hwnd, &ps);

			pRenderTarget->BeginDraw();

			pRenderTarget->Clear(D2D1::ColorF(0x000000));

			hr = pRenderTarget->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
				DiscardGraphicsResources();

			EndPaint(m_hwnd, &ps);
		}

		painting = FALSE;
	}

	void MainWindow::Resize() {
		if (pRenderTarget != NULL) {
			RECT rc;
			GetClientRect(m_hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			pRenderTarget->Resize(size);
			// Call grapher. Send vertices.
			InvalidateRect(m_hwnd, NULL, FALSE);
		}
	}

	LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (closing == 1) {
			closing = 2;
			DiscardGraphicsResources();
			PostQuitMessage(0);
			return 0;
		}

		switch (uMsg) {

		case WM_SETCURSOR:
			if (hasLoaded == TRUE) {
				SetCursor(niceArrow);
				return TRUE;
			}
			break;

		case WM_CREATE:
			return OnCreate();

		case WM_DESTROY:
			DiscardGraphicsResources();
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			OnPaint();
			hasLoaded = TRUE;
			return 0;

		case WM_SIZE:
			Resize();
			return 0;

		case WM_KEYDOWN:
			isKeyBeingPressedOrReleased = PRESSED;
		case WM_KEYUP:
			Controls::ParseKeyEvent(wParam, isKeyBeingPressedOrReleased);
			isKeyBeingPressedOrReleased = RELEASED;
			break;
		}

		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}

	int MainWindow::OnCreate() {
		std::srand((unsigned int)std::time(nullptr));

		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactory)))
			return -1;

		if (Controls::ControlsMain(&m_hwnd, &closing) == 0)
			return -1;

		niceArrow = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_arrow.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		resizeTopBot = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_ns.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		resizeLeftRight = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_ew.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		resizeCornerNESW = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_nesw.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		resizeCornerNWSE = (HCURSOR)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_nwse.cur", IMAGE_CURSOR, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);


		return 0;
	}

}
