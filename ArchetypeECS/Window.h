#pragma once
#include "SDL_Wrapper.h"
#include "Renderer.h"

/// <summary>
/// Struct to load window data in from JSON
/// </summary>
struct WindowData
{
	std::string title;
	int xPosition = 0;
	int yPosition = 0;
	int width = 0;
	int height = 0;
	Uint32 flags = 0;
};

/// <summary>
/// Class to wrap window calls in
/// </summary>
class Window
{
public:
	Window(WindowData windowData);

	bool IsValid() const;

	bool CreateRenderer(Renderer& renderer, int index, Uint32 flags) const;

//private:
	const SDL2::WindowPtr _windowPtr;
};
