#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "basewin.h"

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class MainWindow : public BaseWindow<MainWindow>
{
    ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget;
    ID2D1SolidColorBrush    *pBrushEllipse;
    ID2D1SolidColorBrush    *pBrushLine;
    ID2D1SolidColorBrush    *pBrushText;
    D2D1_COLOR_F            colorLine = D2D1::ColorF(0xB40000);
    D2D1_ELLIPSE            ellipse;

    const D2D1_RECT_F rcText = D2D1::RectF(10.0f, 10.0f, 1000.0f, 1000.0f);
    CHAR buff[13];
    WCHAR text[13];

    IDWriteFactory      *pWriteFactory;
    IDWriteTextFormat   *pWriteTextFormat;

    size_t  multiplier = 0;
    size_t  step = 1;
    float   fMultiplier = 0.0f;
    float   fStep = 0.1f;
    BOOL    usingArrowKeys = FALSE;
    BOOL    isAutomated = FALSE;
    BOOL    mtResize = FALSE;
    BOOL    useFloat = FALSE;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();
    void    Increment();
    void    Decrement();
    void    Reset();
    void    Automate();
    int     OnCreate();

    std::thread t;

public:

    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrushEllipse(NULL), pBrushLine(NULL), pBrushText(NULL), pWriteFactory(NULL), pWriteTextFormat(NULL)
    {
    }

    PCWSTR  ClassName() const { return L"Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // _MAINWINDOW_H_
