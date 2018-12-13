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
    D2D1_ELLIPSE            ellipse;

    const D2D1_RECT_F rcText = D2D1::RectF(10.0f, 130.0f, 1000.0f, 1000.0f);
    CHAR buff[7];
    WCHAR text[7];

    IDWriteFactory      *pWriteFactory;
    IDWriteTextFormat   *pWriteTextFormat;

    size_t  vertices = 10;
    //size_t  step = 1;
    BOOL    isAutomated = FALSE;
    BOOL    mtResize = FALSE;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();
    void    Automate();

    HWND btn_inc;
    HWND btn_dec;
    HWND btn_rst;
    HWND btn_auto;

    objs::Button btn;
    
    std::thread t;

public:

    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrushEllipse(NULL), pBrushLine(NULL), pWriteFactory(NULL), pWriteTextFormat(NULL)
    {
    }

    PCWSTR  ClassName() const { return L"Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif //_MAINWINDOW_H_
