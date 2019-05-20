#include "pch.h"

#include "Grapher.h"

namespace proj {

	void Grapher::_draw() {
		drawing = true;
		while (isThreadActive) {
			if (mustResize == true) {
				mustResize = false;
				if (pRenderTarget != NULL) {
					D2D1_SIZE_U size = D2D1::SizeU(LOWORD(newSize), HIWORD(newSize));

					pRenderTarget->Resize(size);
					// Parse vertices or whatever.
					InvalidateRect(*hWnd, NULL, false);
				}
			}
			else {
				HRESULT hr = CreateGraphicsResources();
				if (SUCCEEDED(hr)) {
					PAINTSTRUCT ps;
					BeginPaint(*hWnd, &ps);

					pRenderTarget->BeginDraw();

					pRenderTarget->Clear(D2D1::ColorF(0x080808));

					hr = pRenderTarget->EndDraw();
					if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
						pFactory->Release();
						pRenderTarget->Release();
					}

					EndPaint(*hWnd, &ps);
				}
			}
		}
		drawing = false;
	}

	HRESULT Grapher::CreateGraphicsResources() {
		HRESULT hr = S_OK;
		if (pRenderTarget == NULL) {
			RECT rc;
			GetClientRect(*hWnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(*hWnd, size),
				&pRenderTarget);

			if (SUCCEEDED(hr)) {
				// Parse vertices or whatever.
			}
		}
		return hr;
	}

	void Grapher::DrawVertices(std::atomic<float>* vertexArray) {
		vertices = vertexArray;
	}

	void Grapher::Resize(LPARAM lParam) {
		if (mustResize == false) {
			this->newSize = lParam;
			mustResize = true;
		}
	}

	Grapher::Grapher(HWND* hWnd) {
		this->hWnd = hWnd;
	}

	Grapher::~Grapher() {
		while (drawing)
			isThreadActive = false;

		if (pFactory) {
			pFactory->Release();
			pFactory = nullptr;
		}
		if (pRenderTarget) {
			pRenderTarget->Release();
			pRenderTarget = nullptr;
		}
	}

	void Grapher::Init() {
		if (drawing == false) {
			isThreadActive = true;

			if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
				FatalAppExit(0, L"An error ocurred trying to initialize the graphics thread.\r\nProgram will shut down.");

			_t = std::thread(&Grapher::_draw, this);
			_t.detach();
		}
	}

}
