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

        if (useFloat == FALSE)
            _gcvt_s(buff, 7, (vertices / 10.0f), ((vertices >= 100) ? 4 : 3));
        else
            _gcvt_s(buff, 7, fVertices, ((fVertices >= 10) ? 4 : 3));

        mbstowcs_s(NULL, text, 7, buff, 7);

        pRenderTarget->DrawTextW(text, (UINT32)wcslen(text), pWriteTextFormat, rcText, pBrushLine);

        if (useFloat == FALSE)
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
        else
            for (size_t i = 0; i < fVertices; i++)
            {
                for (size_t j = 1; j < fVertices - i; j++)
                {
                    pRenderTarget->DrawLine(
                        D2D1::Point2F(
                            ellipse.radiusX * cosf(i * 2.0f * (float)M_PI / fVertices) + ellipse.point.x,
                            ellipse.radiusY * -sinf(i * 2.0f * (float)M_PI / fVertices) + ellipse.point.y),
                        D2D1::Point2F(
                            ellipse.radiusX * cosf((j + i) * 2.0f * (float)M_PI / fVertices) + ellipse.point.x,
                            ellipse.radiusY * -sinf((j + i) * 2.0f * (float)M_PI / fVertices) + ellipse.point.y),
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
            Increment();
            return 0;

        case VK_DOWN:
            Decrement();
            return 0;

        case 0x41:      // A
            t = std::thread(&MainWindow::Automate, this);
            t.detach();
            return 0;

        case 0x52:      // R
            Reset();
            return 0;

        case 0x57:      // W - intended to use with CTRL to close the window.
            if (GetKeyState(VK_LCONTROL) & 0x8000)
            {
                isAutomated = FALSE;
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
    vertices = (vertices >= 500) ? 10 : vertices + 1;
    //OnPaint();        //////////////////////// WHY DOES IT WORK!!!!!!??????????????????????????????????
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::Decrement()
{
    vertices = (vertices < 11) ? 500 : vertices - 1;
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
    vertices = 10;

    OnPaint();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::Automate()
{
    useFloat = TRUE;

    isAutomated = TRUE;

    fVertices = 1.0f;
    fStep = 0.1f;
    while (fVertices < 50.0f && isAutomated == TRUE)
    {
        fVertices += fStep;
        fStep -= 0.0001f;

        if (mtResize)
        {
            Resize();
            mtResize = FALSE;
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }

    while (fVertices > 1.0f && isAutomated == TRUE)
    {
        fVertices -= fStep;
        fStep += 0.0001f;

        if (mtResize)
        {
            Resize();
            mtResize = FALSE;
        }

        OnPaint();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    fVertices = 1.0f;
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
