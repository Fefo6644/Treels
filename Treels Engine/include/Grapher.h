#ifndef _GRAPHER_H_
#define _GRAPHER_H_

namespace proj {

	class Grapher {

	public:
		Grapher(HWND* hWnd);
		~Grapher();

		void Init();
		void Resize(LPARAM lParam);
		void DrawVertices(std::atomic<float>* vertexArray);

	private:
		HWND* hWnd = nullptr;

		HRESULT CreateGraphicsResources();

		ID2D1Factory7* pFactory = nullptr;
		ID2D1HwndRenderTarget* pRenderTarget = nullptr;

		std::atomic<bool> drawing = false;
		std::atomic<bool> mustResize = false;
		std::atomic<LPARAM> newSize = NULL;
		std::atomic<float>* vertices = nullptr;

		std::thread _t = std::thread();
		std::atomic<bool> isThreadActive = false;

		void _draw();

	};

}

#endif
