#include "SDL_Wrapper.h"
#include <SDL2/SDL_image.h>

namespace SDL2
{
	WindowPtr CreateWindow(const char* title,
		int x, int y, int w,
		int h, Uint32 flags)
	{
		return WindowPtr(SDL_CreateWindow(title, x, y, w, h, flags), Deleter());
	}

	RendererPtr CreateRenderer(const WindowPtr& window, int index, Uint32 flags)
	{
		return RendererPtr(SDL_CreateRenderer(window.get(), index, flags), Deleter());
	}

	SurfacePtr LoadImage(const char* path)
	{
		return SurfacePtr(IMG_Load(path), Deleter());
	}

	bool Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "SDL failed to intialise: " << SDL_GetError() << std::endl;
			return false;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			std::cout << "SDL Image failed to intialise: " << IMG_GetError() << std::endl;
			return false;
		}

		return true;
	}


	void Quit()
	{
		IMG_Quit();
		SDL_Quit();
	}

	namespace
	{
		Renderer* rendererPtr = nullptr;
		Window* windowPtr = nullptr;
	}

	void StorePointers(Renderer* renderer, Window* window)
	{
		rendererPtr = renderer;
		windowPtr = window;
	}
	Renderer* GetRenderer()
	{
		return rendererPtr;
	}
	Window* GetWindow()
	{
		return windowPtr;
	}
}