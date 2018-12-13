#ifndef _BASEWIN_H_
#define _BASEWIN_H_

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT *pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        if (pThis)
            return pThis->HandleMessage(uMsg, wParam, lParam);
        else
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    BaseWindow() : m_hwnd(NULL) { }

    BOOL Create(PCWSTR lpWindowName,
        DWORD dwStyle,
        int x = 0,
        int y = 0,
        int nWidth = 400,
        int nHeight = 400,
        HWND hWndParent = 0,
        HMENU hMenu = 0)
    {
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindow(
            ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Window() const { return m_hwnd; }

protected:

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};

#endif //_BASEWIN_H_
