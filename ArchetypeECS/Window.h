#pragma once
#include "SDL_Wrapper.h"
#include "Renderer.h"

/// <summary>
/// Struct to load window data in from JSON
/// </summary>
struct WindowData
{
	std::string title;
	int xPosition;
	int yPosition;
	int width;
	int height;
	Uint32 flags;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowData, title, xPosition, yPosition, width, height, flags);

/// <summary>
/// Class to wrap window calls in
/// </summary>
class Window
{
public:
	Window(WindowData windowData);

	bool IsValid() const;

	bool CreateRenderer(Renderer& renderer, int index, Uint32 flags) const;

private:
	const SDL2::WindowPtr _windowPtr;
};
