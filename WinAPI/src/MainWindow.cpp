#include "pch.h"

#include "MainWindow.h"

void MainWindow::CalculateLayout()
{
    if (pRenderTarget != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 2;
        const float radius = min(x, y);
        ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F colorEllipse = D2D1::ColorF(0.75f, 0.75f, 0.75f);
            hr = pRenderTarget->CreateSolidColorBrush(colorEllipse, &pBrushEllipse);

            if (SUCCEEDED(hr))
            {
                const D2D1_COLOR_F colorLine = D2D1::ColorF(0.0f, 0.0f, 0.0f);
                hr = pRenderTarget->CreateSolidColorBrush(colorLine, &pBrushLine);

                if (SUCCEEDED(hr))
                {
                    pWriteFactory->CreateTextFormat(
                        L"Arial",
                        NULL,
                        DWRITE_FONT_WEIGHT_NORMAL,
                        DWRITE_FONT_STYLE_NORMAL,
                        DWRITE_FONT_STRETCH_NORMAL,
                        24.0f * 96.0f / 72.0f,
                        L"en-EN",
                        &pWriteTextFormat);

                    if (SUCCEEDED(hr))
                        CalculateLayout();
                }
            }
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrushEllipse);
    SafeRelease(&pBrushLine);
    SafeRelease(&pFactory);
    SafeRelease(&pWriteFactory);
    SafeRelease(&pWriteTextFormat);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
        pRenderTarget->DrawEllipse(ellipse, pBrushEllipse);

        _gcvt_s(buff, 7, (vertices / 10.0f), ((vertices >= 100) ? 4 : 3));
        mbstowcs_s(NULL, text, 7, buff, 7);

        pRenderTarget->DrawTextW(text, (UINT32)wcslen(text), pWriteTextFormat, rcText, pBrushLine);

        for (size_t i = 0; i < (vertices / 10.0f); i++)
        {
            for (size_t j = 1; j < (vertices / 10.0f) - i; j++)
            {
                pRenderTarget->DrawLine(
                    D2D1::Point2F(
                        ellipse.radiusX * cosf(i * 2.0f * (float)M_PI / (vertices / 10.0f)) + ellipse.point.x,
                        ellipse.radiusY * -sinf(i * 2.0f * (float)M_PI / (vertices / 10.0f)) + ellipse.point.y),
                    D2D1::Point2F(
                        ellipse.radiusX * cosf((j + i) * 2.0f * (float)M_PI / (vertices / 10.0f)) + ellipse.point.x,
                        ellipse.radiusY * -sinf((j + i) * 2.0f * (float)M_PI / (vertices / 10.0f)) + ellipse.point.y),
                    pBrushLine);
            }
        }

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            DiscardGraphicsResources();

        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactory)))
            return -1;

        if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory))))
            return -1;

        if (btn.Create(m_hwnd, L"aaaaaa", objs::Button::ClassicButton, objs::Location(10, 200), objs::Size(60, 20)) == FALSE)
            return -1;

        btn_inc = CreateWindow(
            L"BUTTON", L"Increase",
            WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10,
            75, 25,
            m_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), NULL);

        btn_dec = CreateWindow(
            L"BUTTON", L"Decrease",
            WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 40,
            75, 25,
            m_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), NULL);

        btn_rst = CreateWindow(
            L"BUTTON", L"Reset",
            WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 70,
            75, 25,
            m_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), NULL);

        btn_auto = CreateWindow(
            L"BUTTON", L"Automate",
            WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 100,
            75, 25,
            m_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), NULL);

        return 0;

    case WM_CLOSE:
        isAutomated = FALSE;
        break;

    case WM_DESTROY:
        DiscardGraphicsResources();
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        if (isAutomated == FALSE)
            OnPaint();
        return isAutomated;

    case WM_SIZE:
        if (isAutomated == FALSE)
            Resize();
        else
            mtResize = TRUE;
        return isAutomated;

    case WM_COMMAND:
    case BN_CLICKED:
        if ((HWND)lParam == btn_inc)
            vertices = (vertices >= 500) ? 10 : vertices + 1;
        else if ((HWND)lParam == btn_dec)
            vertices = (vertices < 11) ? 500 : vertices - 1;
        else if ((HWND)lParam == btn_rst)
        {
            if (isAutomated == TRUE)
            {
                isAutomated = FALSE;
                Sleep(20);
            }

            vertices = 10;
        }
        else if ((HWND)lParam == btn_auto)
        {
            t = std::thread(&MainWindow::Automate, this);
            t.detach();
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);

        return 0;

        break;

    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::Automate()
{
    isAutomated = TRUE;

    Button_Enable(btn_inc, FALSE);
    Button_Enable(btn_dec, FALSE);
    Button_Enable(btn_auto, FALSE);
    Button_SetText(btn_rst, L"Stop");

    vertices = 10;
    //step = 1;
    while (vertices < 500 && isAutomated == TRUE)
    {
        vertices++;
        //vertices += step;
        //step -= 1;

        if (mtResize)
        {
            Resize();
            mtResize = FALSE;
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }

    while (vertices > 10 && isAutomated == TRUE)
    {
        vertices--;
        //vertices -= step;
        //step += 1;

        if (mtResize)
        {
            Resize();
            mtResize = FALSE;
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    vertices = 10;
    //step = 1;

    Button_Enable(btn_inc, TRUE);
    Button_Enable(btn_dec, TRUE);
    Button_Enable(btn_auto, TRUE);
    Button_SetText(btn_rst, L"Reset");

    isAutomated = FALSE;
}
