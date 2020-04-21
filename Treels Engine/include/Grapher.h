#ifndef _GRAPHER_H_
#define _GRAPHER_H_

namespace Treels {
    namespace Grapher {
        class Grapher {
        public:
            Grapher(HWND hWnd) : hWnd(hWnd) {}
            ~Grapher();

            void init();
            float getRefreshRate() const;
            POINT getDisplayRes() const;

        private:
            POINT resolution{};

            ::std::chrono::time_point<::std::chrono::steady_clock, ::std::chrono::duration<double, ::std::milli>> start = {};
            ::std::chrono::duration<double, ::std::milli> timePassed = {};
            ::std::chrono::duration<float, ::std::milli> displayPeriod = {};

            HWND hWnd = nullptr;

            ID2D1Factory* pFactory = nullptr;
            ID2D1HwndRenderTarget* pRenderTarget = nullptr;
            ID2D1SolidColorBrush* pBrush = nullptr;
        };
    }
}
#endif // !_GRAPHER_H_
