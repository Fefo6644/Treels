#ifndef _GRAPHER_H_
#define _GRAPHER_H_

namespace Treels {
	namespace Grapher {
		struct OrderingIndex {
			static constexpr bool Compare(const OrderingIndex& a, const OrderingIndex& b) {
				return a.obj->zOrder < b.obj->zOrder;
			}
			Objects::Object* obj = nullptr;
			GUID objGuid = { 0 };
		};

		class Grapher {
		public:
			Grapher(HWND hWnd, Objects::Color backColor = Objects::Color(0.0f, 0.0f, 0.0f, 1.0f));
			~Grapher();

			void Init();
			void Resize(LPARAM lParam);
			void SetBackgroundColor(float r, float g, float b);
			void LoadCircles(::std::vector<Objects::Circle>* circlesArray);
			void UnloadCircles();
			void LoadRectangles(::std::vector<Objects::Rectangle>* rectanglesArray);
			void UnloadRectangles();
			void LoadLines(::std::vector<Objects::Line>* linesArray);
			void UnloadLines();
			void Unload();
			void Refresh();
			void Draw();
			float GetRefreshRate();

		private:
			D2D1_POINT_2F center;
			D2D1_COLOR_F bgColor;
			RECT rc;

			::std::chrono::time_point<::std::chrono::steady_clock, ::std::chrono::duration<double, ::std::milli>> start;
			::std::chrono::duration<double, ::std::milli> timePassed;
			::std::chrono::duration<float, ::std::milli> displayPeriod;

			HWND hWnd;

			ID2D1Factory* pFactory;
			ID2D1HwndRenderTarget* pRenderTarget;
			ID2D1SolidColorBrush* pBrush;

			::std::vector<Objects::Circle>* circles;
			::std::vector<Objects::Rectangle>* rectangles;
			::std::vector<Objects::Line>* lines;

			::std::vector<OrderingIndex> oi;

			void DrawCircle(Objects::Circle& circle);
			void DrawRectangle(Objects::Rectangle& rectangle);
			void DrawLine(Objects::Line& line);
		};
	}
}
#endif // !_GRAPHER_H_
