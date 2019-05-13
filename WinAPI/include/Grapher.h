#ifndef _GRAPHER_H_
#define _GRAPHER_H_

namespace proj {

	class Grapher {

	public:
		Grapher(HWND* hWnd);
		~Grapher();

		void Init();
		void Resize();
		void DrawVertices(float* vertexArray);

	private:
		HWND* hWnd = nullptr;

		HRESULT CreateGraphicsResources();

		ID2D1Factory7* pFactory = nullptr;
		ID2D1HwndRenderTarget* pRenderTarget = nullptr;

		std::atomic<bool> drawing = false;
		std::atomic<bool> mustResize = false;

		std::thread _t = std::thread();
		std::atomic<bool> isThreadActive = false;

		void _draw();

	};

}

#endif
