#include "pch.h"

#include "Grapher.h"

using namespace ::std::chrono_literals;

namespace Treels {
    namespace Grapher {
        /* Return the display frequency */
        float Grapher::getRefreshRate() const { return (1.0f / displayPeriod.count()); }

        /* Return the display resolution */
        POINT Grapher::getDisplayRes() const { return resolution; }

        /* dtor */
        Grapher::~Grapher() {
            CoUninitialize();
        }

        /* Renderer initialization */
        void Grapher::init() {
            /*
            · Call initialization for use of the COM library.
            · Create the imaging factory for use of bitmaps.
            · Create the D2D1 Factory to create the render target and brushes.
            */
            ::std::wstringstream errMessage;
            errMessage << L"An error ocurred trying to initialize graphics. Program will shut down. Error code: 0x";

            HRESULT result = CoInitializeEx(NULL,
                COINIT::COINIT_SPEED_OVER_MEMORY);
            if (FAILED(result)) {
                errMessage << ::std::hex << result;
                MessageBox(hWnd, errMessage.str().c_str(), NULL, MB_ICONERROR);
                ::std::exit(-1);
            }

            result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
            if (FAILED(result)) {
                errMessage << ::std::hex << result;
                MessageBox(hWnd, errMessage.str().c_str(), NULL, MB_ICONERROR);
                ::std::exit(-1);
            }

            D2D1_SIZE_U size;
            RECT rc;
            GetClientRect(hWnd, &rc);
            size = D2D1::SizeU(rc.right, rc.bottom);

            /* Create the "canvas" */
            pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hWnd, size),
                &pRenderTarget);

            /* Create the brushes */
            pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x00000000), &pBrush);

            /* Getting the display frequency */
            DEVMODE displaySettings = { 0 };
            EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &displaySettings);
            resolution = { static_cast<LONG>(displaySettings.dmPelsWidth),
                static_cast<LONG>(displaySettings.dmPelsHeight) };
            displayPeriod = ::std::chrono::duration<float, ::std::milli>(1.0f / displaySettings.dmDisplayFrequency);
        }
    }
}
