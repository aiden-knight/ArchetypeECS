#include "Window.h"

Window::Window(WindowData windowData) :
	_windowPtr{ SDL2::CreateWindow(	windowData.title.c_str(),
									windowData.xPosition, 
									windowData.yPosition, 
									windowData.width,
									windowData.height,
									windowData.flags) }
{
	if (_windowPtr == nullptr)
	{
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
	}
}

bool Window::IsValid() const
{
	return _windowPtr != nullptr;
}

bool Window::CreateRenderer(Renderer& renderer, int index, Uint32 flags) const
{
	if (!IsValid()) return false;

	renderer = std::move(Renderer(_windowPtr, -1, SDL_RENDERER_ACCELERATED));
	return renderer.IsValid();
}
