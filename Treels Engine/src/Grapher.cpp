#include "pch.h"

#include "Objects.h"
#include "Grapher.h"

using namespace std::chrono_literals;

namespace grph {
	void Grapher::Draw() {
		if (mustResize == true) {
			mustResize = false;
			if (pRenderTarget != nullptr) {
				D2D1_SIZE_U size = D2D1::SizeU(LOWORD(newSize), HIWORD(newSize));

				pRenderTarget->Resize(size);
				InvalidateRect(*hWnd, NULL, false);
			}
		}
		else {
			start = std::chrono::steady_clock::now();

			HRESULT hr;
			PAINTSTRUCT ps;
			BeginPaint(*hWnd, &ps);

			pRenderTarget->BeginDraw();

			pRenderTarget->Clear(bgColor);

			/*---------------- Draw Circles ----------------*/

			if (circles != nullptr)
				for (unsigned int i = 0; i < circles->size(); i++) {
					if (circles->at(i)->drawFill) {
						pBrushFill->SetColor(circles->at(i)->colorFill.color);
						pRenderTarget->FillEllipse(circles->at(i)->circle, pBrushFill);
					}
					if (circles->at(i)->drawOutline) {
						pBrushOutline->SetColor(circles->at(i)->colorOutline.color);
						pRenderTarget->DrawEllipse(circles->at(i)->circle, pBrushOutline);
					}
				}

			/*---------------- Draw Rectangles ----------------*/

			if (rectangles != nullptr)
				for (unsigned int i = 0; i < rectangles->size(); i++) {
					if (rectangles->at(i)->drawFill) {
						pBrushFill->SetColor(rectangles->at(i)->colorFill.color);
						pRenderTarget->FillRectangle(rectangles->at(i)->rectangle, pBrushFill);
					}
					if (rectangles->at(i)->drawOutline) {
						pBrushOutline->SetColor(rectangles->at(i)->colorOutline.color);
						pRenderTarget->DrawRectangle(rectangles->at(i)->rectangle, pBrushOutline);
					}
				}

			/*---------------- Draw Lines ----------------*/

			if (lines != nullptr)
				for (unsigned int i = 0; i < lines->size(); i++) {
					if (lines->at(i)->draw) {
						pBrushOutline->SetColor(lines->at(i)->color.color);
						pRenderTarget->DrawLine(lines->at(i)->from.point, lines->at(i)->to.point, pBrushOutline);
					}
				}

			hr = pRenderTarget->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
				pFactory->Release();
				pRenderTarget->Release();
			}

			EndPaint(*hWnd, &ps);

			timePassed = std::chrono::steady_clock::now() - start;
			if (timePassed < displayPeriod)
				std::this_thread::sleep_for(displayPeriod - timePassed);
		}
	}

	void Grapher::LoadCircles(std::vector<objs::Circle*>* circlesArray) {
		circles = circlesArray;
	}

	void Grapher::UnloadCircles() {
		circles = nullptr;
	}

	void Grapher::LoadRectangles(std::vector<objs::Rectangle*>* rectanglesArray) {
		rectangles = rectanglesArray;
	}

	void Grapher::UnloadRectangles() {
		rectangles = nullptr;
	}

	void Grapher::LoadLines(std::vector<objs::Line*>* linesArray) {
		lines = linesArray;
	}

	void Grapher::UnloadLines() {
		lines = nullptr;
	}

	void Grapher::Unload() {
		UnloadCircles();
		UnloadRectangles();
		UnloadLines();
	}

	void Grapher::Refresh() {

	}

	void Grapher::Resize(LPARAM lParam) {
		if (mustResize == false) {
			newSize = lParam;
			mustResize = true;
		}
	}

	void Grapher::SetBackgroundColor(float r, float g, float b) {
		bgColor.r = r;
		bgColor.g = g;
		bgColor.b = b;
	}

	float Grapher::GetRefreshRate() {
		return (1.0f / displayPeriod.count());
	}

	Grapher::Grapher(HWND* hWnd, objs::Color backColor) {
		this->hWnd = hWnd;

		pFactory = nullptr;
		pRenderTarget = nullptr;
		bgColor = D2D1::ColorF(backColor.color.r, backColor.color.g, backColor.color.b);

		mustResize = false;
		newSize = NULL;
		circles = nullptr;
		rectangles = nullptr;
		lines = nullptr;

		start = {};
		timePassed = {};
		displayPeriod = {};
	}

	Grapher::~Grapher() {
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
		HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

		if (FAILED(res)) {
			std::wstringstream errMessage;
			errMessage << L"An error ocurred trying to initialize graphics. Program will shut down. Error code: 0x" << std::hex << res;
			FatalAppExit(0, errMessage.str().c_str());
		}

		D2D1_SIZE_U size;
		RECT rc;
		GetClientRect(*hWnd, &rc);
		size = D2D1::SizeU(rc.right, rc.bottom);

		pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(*hWnd, size),
			&pRenderTarget);

		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x00000000), &pBrushFill);
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x00000000), &pBrushOutline);

		DEVMODE displaySettings = {};
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &displaySettings);
		displayPeriod = std::chrono::duration<float, std::milli>(1.0 / displaySettings.dmDisplayFrequency);
	}
}
