#ifndef _GRAPHER_H_
#define _GRAPHER_H_

namespace grph {

	class Grapher {
	public:
		Grapher(HWND* hWnd, objs::Color backColor = objs::Color());
		~Grapher();

		void Init();
		void Resize(LPARAM lParam);
		void DrawCircles(std::vector<std::atomic<objs::Circle>>* circlesArray);
		void DrawRectangles(std::vector<std::atomic<objs::Rectangle>>* rectanglesArray);
		void DrawLines(std::vector<std::atomic<objs::Line>>* linesArray);

	private:
		HWND* hWnd;

		ID2D1Factory* pFactory;
		ID2D1HwndRenderTarget* pRenderTarget;
		ID2D1SolidColorBrush* pBrushOutline;
		ID2D1SolidColorBrush* pBrushFill;
		D2D1_COLOR_F bgColor;

		std::atomic<bool> drawing;
		std::atomic<bool> mustResize;
		std::atomic<bool> isThreadActive;
		std::atomic<LPARAM> newSize;
		std::vector<std::atomic<objs::Circle>>* circles;
		std::vector<std::atomic<objs::Rectangle>>* rectangles;
		std::vector<std::atomic<objs::Line>>* lines;
		std::chrono::time_point<std::chrono::system_clock> timeBeg, timeEnd;
		DEVMODE displaySettings;

		std::thread _drawingThread;

		void _draw();
	};

}

#endif // !_GRAPHER_H_
