#include "Renderer.h"
#include "Window.h"
#include "JSONLoader.h"

/// <summary>
/// Contains the loop for polling SDL events
/// </summary>
/// <returns>True if should quit</returns>
bool PollEvents();

void Update();
void Render(Renderer& renderer);

int main(int argc, char** argv)
{
	// Initialise
	bool quit{ !SDL2::Init() };

	// Create window and renderer
	WindowData data = JSONLoader::Load<WindowData>("Assets/json/Init.json", "WindowData");
	Window window{ data };
	Renderer renderer;
	if (!window.CreateRenderer(renderer, -1, SDL_RENDERER_ACCELERATED)) quit = true;

	// Create game objects
	if (!quit)
	{
		// store pointers to renderer and window in case components need them
		SDL2::StorePointers(&renderer, &window);
	}

	// Game Loop
	while (!quit)
	{
		// Poll events
		quit = PollEvents();

		// Update
		Update();

		// Render
		Render(renderer);
	}

	// Quit and destruct
	SDL2::Quit();
	return 0;
}

bool PollEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return true;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			break;
		}
	}

	return false;
}

void Update()
{
	static Uint64 previousTime;
	Uint64 deltaTime = SDL_GetTicks64() - previousTime;



	previousTime = SDL_GetTicks64();
}

void Render(Renderer& renderer)
{
	renderer.Clear();



	renderer.Present();
}