#ifndef _GRAPHER_H_
#define _GRAPHER_H_

namespace grph {
	class Grapher {
	public:
		Grapher(HWND* hWnd, objs::Color backColor = objs::Color(0.0f, 0.0f, 0.0f, 1.0f));
		~Grapher();

		void Init();
		void Resize(LPARAM lParam);
		void SetBackgroundColor(float r, float g, float b);
		void LoadCircles(std::vector<objs::Circle*>* circlesArray);
		void UnloadCircles();
		void LoadRectangles(std::vector<objs::Rectangle*>* rectanglesArray);
		void UnloadRectangles();
		void LoadLines(std::vector<objs::Line*>* linesArray);
		void UnloadLines();
		void Unload();
		void Refresh();
		void Draw();
		float GetRefreshRate();


	private:
		D2D1_COLOR_F bgColor;

		std::atomic<LPARAM> newSize;
		std::atomic<bool> mustResize;

		std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::milli>> start;
		std::chrono::duration<double, std::milli> timePassed;
		std::chrono::duration<float, std::milli> displayPeriod;

		HWND* hWnd;

		ID2D1Factory* pFactory;
		ID2D1HwndRenderTarget* pRenderTarget;
		ID2D1SolidColorBrush* pBrushOutline;
		ID2D1SolidColorBrush* pBrushFill;

		std::vector<objs::Circle*>* circles;
		std::vector<objs::Rectangle*>* rectangles;
		std::vector<objs::Line*>* lines;
	};
}

#endif // !_GRAPHER_H_
