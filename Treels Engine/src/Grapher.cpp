#include "pch.h"

#include "Objects.h"
#include "Grapher.h"

namespace grph {

	void Grapher::_draw() {
		drawing = true;

		while (isThreadActive) {
			if (mustResize == true) {
				mustResize = false;
				if (pRenderTarget != NULL) {
					D2D1_SIZE_U size = D2D1::SizeU(LOWORD(newSize), HIWORD(newSize));

					pRenderTarget->Resize(size);
					InvalidateRect(*hWnd, NULL, false);
				}
			}
			else {
				timeBeg = std::chrono::system_clock::now();

				PAINTSTRUCT ps;
				BeginPaint(*hWnd, &ps);

				pRenderTarget->BeginDraw();

				pRenderTarget->Clear(bgColor);

				/*---------------- Draw Circles ----------------*/

				for (int i = 0; i < (*circles).size(); i++) {
					if ((*circles)[i].load().drawFill) {
						pBrushFill->SetColor((*circles)[i].load().colorFill.GetD2D1Color());
						pRenderTarget->FillEllipse((*circles)[i].load().GetD2D1Ellipse(), pBrushFill);
					}
					if ((*circles)[i].load().drawOutline) {
						pBrushOutline->SetColor((*circles)[i].load().colorOutline.GetD2D1Color());
						pRenderTarget->DrawEllipse((*circles)[i].load().GetD2D1Ellipse(), pBrushOutline);
					}
				}

				/*---------------- Draw Rectangles ----------------*/

				for (int i = 0; i < (*rectangles).size(); i++) {
					if ((*rectangles)[i].load().drawFill) {
						pBrushFill->SetColor((*rectangles)[i].load().colorFill.GetD2D1Color());
						pRenderTarget->FillRectangle((*rectangles)[i].load().GetD2D1Rect(), pBrushFill);
					}
					if ((*rectangles)[i].load().drawOutline) {
						pBrushOutline->SetColor((*rectangles)[i].load().colorOutline.GetD2D1Color());
						pRenderTarget->DrawRectangle((*rectangles)[i].load().GetD2D1Rect(), pBrushOutline);
					}
				}

				/*---------------- Draw Lines ----------------*/

				for (int i = 0; i < (*circles).size(); i++) {
					if ((*lines)[i].load().draw) {
						pBrushOutline->SetColor((*lines)[i].load().color.GetD2D1Color());
						pRenderTarget->DrawLine((*lines)[i].load().from.GetD2D1Point(), (*lines)[i].load().to.GetD2D1Point(), pBrushOutline);
					}
				}

				HRESULT hr = pRenderTarget->EndDraw();
				if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
					pFactory->Release();
					pRenderTarget->Release();
				}

				EndPaint(*hWnd, &ps);

				timeEnd = std::chrono::system_clock::now();
				auto duration = timeEnd - timeBeg;
				if (duration.count() < 1000 / displaySettings.dmDisplayFrequency)
					Sleep(1000 / displaySettings.dmDisplayFrequency - (DWORD)duration.count());

			}
		}
		drawing = false;
	}

	void Grapher::DrawCircles(std::vector<std::atomic<objs::Circle>>* circlesArray) {
		circles = circlesArray;
	}

	void Grapher::DrawRectangles(std::vector<std::atomic<objs::Rectangle>>* rectanglesArray) {
		rectangles = rectanglesArray;
	}

	void Grapher::DrawLines(std::vector<std::atomic<objs::Line>>* linesArray) {
		lines = linesArray;
	}

	void Grapher::Resize(LPARAM lParam) {
		if (mustResize == false) {
			newSize = lParam;
			mustResize = true;
		}
	}

	Grapher::Grapher(HWND* hWnd, objs::Color backColor) {
		this->hWnd = hWnd;

		pFactory = nullptr;
		pRenderTarget = nullptr;
		bgColor = D2D1::ColorF(backColor.GetR(), backColor.GetG(), backColor.GetB());

		drawing = false;
		mustResize = false;
		newSize = NULL;
		circles = nullptr;
		rectangles = nullptr;
		lines = nullptr;
		displaySettings = { NULL };

		_drawingThread = std::thread();
		isThreadActive = false;
	}

	Grapher::~Grapher() {
		while (drawing)
			isThreadActive = false;

		if (pFactory != nullptr) {
			pFactory->Release();
			pFactory = nullptr;
		}
		if (pRenderTarget != nullptr) {
			pRenderTarget->Release();
			pRenderTarget = nullptr;
		}
		if (pBrushFill != nullptr) {
			pBrushFill->Release();
			pBrushFill = nullptr;
		}
		if (pBrushOutline != nullptr) {
			pBrushOutline->Release();
			pBrushOutline = nullptr;
		}
	}

	void Grapher::Init() {
		if (drawing == false) {
			isThreadActive = true;

			HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

			if (FAILED(res)) {
				std::wstringstream errMessage;
				errMessage << L"An error ocurred trying to initialize graphics. Program will shut down. Error code: 0x" << std::hex << res;
				FatalAppExit(0, errMessage.str().c_str());
			}

			RECT rc;
			GetClientRect(*hWnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(*hWnd, size),
				&pRenderTarget);

			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x00000000), &pBrushFill);
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x00000000), &pBrushOutline);

			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &displaySettings);

			_drawingThread = std::thread(&Grapher::_draw, this);
			_drawingThread.detach();
		}
	}
}
