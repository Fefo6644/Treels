#include "pch.h"

#include "Objects.h"
#include "Grapher.h"

using namespace std::chrono_literals;

namespace grph {
	/* General drawing function */
	void Grapher::Draw() {
		/* If the window is being resized, take care of that and resize
		and redraw the image as well */
		if (mustResize == true) {
			mustResize = false;
			if (pRenderTarget != nullptr) {
				D2D1_SIZE_U size = D2D1::SizeU(LOWORD(newSize), HIWORD(newSize));

				pRenderTarget->Resize(size);
				InvalidateRect(*hWnd, NULL, false);
			}
		}
		/* Otherwise, draw */
		else {
			/* Measuring the time it takes to draw a single frame */
			start = std::chrono::steady_clock::now();

			HRESULT hr;
			PAINTSTRUCT ps;
			BeginPaint(*hWnd, &ps);

			pRenderTarget->BeginDraw();

			/* Draw background color */
			pRenderTarget->Clear(bgColor);

			/* Based on the zOrder, draw the corresponding object based on its type
			from back to front */
			for (auto& i : oi) {
				if (i.obj->type == objs::Type::ECircle)
					DrawCircle((objs::Circle*)i.obj);
				else if (i.obj->type == objs::Type::ERectangle)
					DrawRectangle((objs::Rectangle*)i.obj);
				else if (i.obj->type == objs::Type::ELine)
					DrawLine((objs::Line*)i.obj);
			}

			/* And if something just failed, well, idk */
			hr = pRenderTarget->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
				pFactory->Release();
				pRenderTarget->Release();
			}

			EndPaint(*hWnd, &ps);

			/* Once the drawing time is measured, if it's less than the display period,
			wait until the period ends */
			timePassed = std::chrono::steady_clock::now() - start;
			if (timePassed < displayPeriod)
				std::this_thread::sleep_for(displayPeriod - timePassed);
		}
	}

	/* Draw a circle object */
	void Grapher::DrawCircle(objs::Circle* circle) {
		if (circle->drawFill) {
			pBrush->SetColor(circle->colorFill.color);
			pRenderTarget->FillEllipse(circle->circle, pBrush);
		}
		if (circle->drawOutline) {
			pBrush->SetColor(circle->colorOutline.color);
			pRenderTarget->DrawEllipse(circle->circle, pBrush);
		}
	}

	/* Draw a rectangle object */
	void Grapher::DrawRectangle(objs::Rectangle* rectangle) {
		if (rectangle->drawFill) {
			pBrush->SetColor(rectangle->colorFill.color);
			pRenderTarget->FillRectangle(rectangle->rectangle, pBrush);
		}
		if (rectangle->drawOutline) {
			pBrush->SetColor(rectangle->colorOutline.color);
			pRenderTarget->DrawRectangle(rectangle->rectangle, pBrush);
		}
	}

	/* Draw a line object */
	void Grapher::DrawLine(objs::Line* line) {
		if (line->draw) {
			pBrush->SetColor(line->color.color);
			pRenderTarget->DrawLine(line->from.point, line->to.point, pBrush);
		}
	}

	/* Load the circles into the renderer */
	void Grapher::LoadCircles(std::vector<objs::Circle*>* circlesArray) {
		circles = circlesArray;
		OrderingIndex temp;
		for (auto& i : *circlesArray) {
			temp.zOrder = i->zOrder;
			temp.obj = i;
			temp.objGuid = i->_guid;
			oi.push_back(temp);
		}

		oi.shrink_to_fit();
	}

	/* Unload the circles from the renderer */
	void Grapher::UnloadCircles() {
		circles = nullptr;
		for (size_t i = 0; i < oi.size(); ++i)
			if (oi[i].obj->type == objs::Type::ECircle)
				oi.erase(oi.begin() + i--);

		oi.shrink_to_fit();
	}

	/* Load the rectangles into the renderer */
	void Grapher::LoadRectangles(std::vector<objs::Rectangle*>* rectanglesArray) {
		rectangles = rectanglesArray;
		OrderingIndex temp;
		for (auto& i : *rectanglesArray) {
			temp.zOrder = i->zOrder;
			temp.obj = i;
			temp.objGuid = i->_guid;
			oi.push_back(temp);
		}

		oi.shrink_to_fit();
	}

	/* Unload the rectangles from the renderer */
	void Grapher::UnloadRectangles() {
		rectangles = nullptr;
		for (size_t i = 0; i < oi.size(); ++i)
			if (oi[i].obj->type == objs::Type::ERectangle)
				oi.erase(oi.begin() + i--);

		oi.shrink_to_fit();
	}

	/* Load the lines into the renderer */
	void Grapher::LoadLines(std::vector<objs::Line*>* linesArray) {
		lines = linesArray;
		OrderingIndex temp;
		for (auto& i : *linesArray) {
			temp.zOrder = i->zOrder;
			temp.obj = i;
			temp.objGuid = i->_guid;
			oi.push_back(temp);
		}

		oi.shrink_to_fit();
	}

	/* Unload the lines from the renderer */
	void Grapher::UnloadLines() {
		lines = nullptr;
		for (size_t i = 0; i < oi.size(); ++i)
			if (oi[i].obj->type == objs::Type::ELine)
				oi.erase(oi.begin() + i--);

		oi.shrink_to_fit();
	}

	/* Unload every object from the renderer */
	void Grapher::Unload() {
		UnloadCircles();
		UnloadRectangles();
		UnloadLines();
	}

	/* Check if an object has been deleted based on local and external GUID
	and reorder based on zOrder */
	void Grapher::Refresh() {
		for (size_t i = 0; i < oi.size(); ++i)
			if (oi[i].objGuid != oi[i].obj->_guid)
				oi.erase(oi.begin() + i--);

		std::sort(oi.begin(), oi.end(), OrderingIndex::Compare);
	}

	/* Get the signal for resizing */
	void Grapher::Resize(LPARAM lParam) {
		if (mustResize == false) {
			newSize = lParam;
			mustResize = true;
		}
	}

	/* Set the background color */
	void Grapher::SetBackgroundColor(float r, float g, float b) {
		bgColor.r = r;
		bgColor.g = g;
		bgColor.b = b;
	}

	/* Return the display frequency */
	float Grapher::GetRefreshRate() {
		return (1.0f / displayPeriod.count());
	}

	/* ctor */
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

	/* dtor */
	Grapher::~Grapher() {
		if (pFactory != nullptr) {
			pFactory->Release();
			pFactory = nullptr;
		}
		if (pRenderTarget != nullptr) {
			pRenderTarget->Release();
			pRenderTarget = nullptr;
		}
		if (pBrush != nullptr) {
			pBrush->Release();
			pBrush = nullptr;
		}
	}

	/* Renderer initialization */
	void Grapher::Init() {
		/* Create the D2D1 Factory to create the render target and brushes */
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

		/* Create the "canvas" */
		pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(*hWnd, size),
			&pRenderTarget);

		/* Create the brushes */
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x00000000), &pBrush);

		/* Getting the display frequency */
		DEVMODE displaySettings = {};
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &displaySettings);
		displayPeriod = std::chrono::duration<float, std::milli>(1.0f / displaySettings.dmDisplayFrequency);
	}
}
