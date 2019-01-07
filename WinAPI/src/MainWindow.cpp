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
                const D2D1_COLOR_F colorText = D2D1::ColorF(0.0f, 0.0f, 0.0f);
                hr = pRenderTarget->CreateSolidColorBrush(colorText, &pBrushText);

                if (SUCCEEDED(hr))
                {
                    hr = pRenderTarget->CreateSolidColorBrush(colorLine, &pBrushLine);

                    if (SUCCEEDED(hr))
                    {
                        hr = pWriteFactory->CreateTextFormat(
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
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrushEllipse);
    SafeRelease(&pBrushLine);
    SafeRelease(&pBrushText);
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

        if (useFloat == FALSE)
            _gcvt_s(buff, 13, (multiplier / 100.0f), 4);
        else
            _gcvt_s(buff, 13, fMultiplier, 4);

        mbstowcs_s(NULL, text, 13, buff, 13);

        pRenderTarget->DrawTextW(text, (UINT32)wcslen(text), pWriteTextFormat, rcText, pBrushText);

        colorLine = D2D1::ColorF(0xB40000);

        if (useFloat == FALSE)
            for (size_t i = 0; i < 360; i++)
            {
                if (i < 60)
                    colorLine.g += 3.0f / 255.0f;
                else if (i < 120)
                    colorLine.r -= 3.0f / 255.0f;
                else if (i < 180)
                    colorLine.b += 3.0f / 255.0f;
                else if (i < 240)
                    colorLine.g -= 3.0f / 255.0f;
                else if (i < 300)
                    colorLine.r += 3.0f / 255.0f;
                else
                    colorLine.b -= 3.0f / 255.0f;

                pBrushLine->SetColor(colorLine);

                pRenderTarget->DrawLine(
                    D2D1::Point2F(
                        ellipse.radiusX * cosf(i * (float)M_PI / 180.0f) + ellipse.point.x,
                        ellipse.radiusY * -sinf(i * (float)M_PI / 180.0f) + ellipse.point.y),
                    D2D1::Point2F(
                        ellipse.radiusX * cosf(i * (float)M_PI / 180.0f * (multiplier / 100.0f)) + ellipse.point.x,
                        ellipse.radiusY * -sinf(i * (float)M_PI / 180.0f * (multiplier / 100.0f)) + ellipse.point.y),
                    pBrushLine);
            }
        else
            for (size_t i = 0; i < 360; i++)
            {
                if (i < 60)
                    colorLine.g += 3.0f / 255.0f;
                else if (i < 120)
                    colorLine.r -= 3.0f / 255.0f;
                else if (i < 180)
                    colorLine.b += 3.0f / 255.0f;
                else if (i < 240)
                    colorLine.g -= 3.0f / 255.0f;
                else if (i < 300)
                    colorLine.r += 3.0f / 255.0f;
                else
                    colorLine.b -= 3.0f / 255.0f;

                pBrushLine->SetColor(colorLine);

                pRenderTarget->DrawLine(
                    D2D1::Point2F(
                        ellipse.radiusX * cosf(i * (float)M_PI / 180.0f) + ellipse.point.x,
                        ellipse.radiusY * -sinf(i * (float)M_PI / 180.0f) + ellipse.point.y),
                    D2D1::Point2F(
                        ellipse.radiusX * cosf(i * (float)M_PI / 180.0f * fMultiplier) + ellipse.point.x,
                        ellipse.radiusY * -sinf(i * (float)M_PI / 180.0f * fMultiplier) + ellipse.point.y),
                    pBrushLine);
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
        return OnCreate();

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

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            if (isAutomated == FALSE)
                Increment();
            return 0;

        case VK_DOWN:
            if (isAutomated == FALSE)
                Decrement();
            return 0;

        case 0x41:      // A
            if (isAutomated == FALSE)
            {
                t = std::thread(&MainWindow::Automate, this);
                t.detach();
            }
            return 0;

        case 0x52:      // R
            Reset();
            return 0;

        case 0x57:      // W - intended to use with CTRL to close the window.
            if (GetKeyState(VK_LCONTROL) & 0x8000)
            {
                isAutomated = FALSE;
                Sleep(25);
                DiscardGraphicsResources();
                PostQuitMessage(0);
                return 0;
            }

        default:
            break;
        }
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::Increment()
{
    multiplier = (multiplier >= 5000) ? 0 : multiplier + step;
    //OnPaint();        //////////////////////// WHY DOES IT WORK!!!!!!??????????????????????????????????
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::Decrement()
{
    multiplier = (multiplier < 1) ? 5000 : multiplier - step;
    //OnPaint();        //////////////////////// WHY DOES IT WORK!!!!!!??????????????????????????????????
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::Reset()
{
    if (isAutomated == TRUE)
    {
        isAutomated = FALSE;
        Sleep(25);
    }
    multiplier = 0;

    OnPaint();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::Automate()
{
    useFloat = TRUE;

    isAutomated = TRUE;

    fMultiplier = 1.0f;
    fStep = 0.1f;
    while (fMultiplier < 50.0f && isAutomated == TRUE)
    {
        fMultiplier += fStep;
        fStep -= 0.0001f;

        if (mtResize)
        {
            Resize();
            mtResize = FALSE;
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }

    while (fMultiplier > 1.0f && isAutomated == TRUE)
    {
        fMultiplier -= fStep;
        fStep += 0.0001f;

        if (mtResize)
        {
            Resize();
            mtResize = FALSE;
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    fMultiplier = 1.0f;
    fStep = 0.1f;

    InvalidateRect(m_hwnd, NULL, FALSE);

    isAutomated = FALSE;

    useFloat = FALSE;
}

int MainWindow::OnCreate()
{
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactory)))
        return -1;

    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory))))
        return -1;

    return 0;
}
