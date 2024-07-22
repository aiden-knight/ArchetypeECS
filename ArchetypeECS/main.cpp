#include "Renderer.h"
#include "Window.h"
#include "JSONLoader.h"

#include "Engine.h"
#include "System.h"

#include <random>

/// <summary>
/// Contains the loop for polling SDL events
/// </summary>
/// <returns>True if should quit</returns>
bool PollEvents();

void Update();
void Render(Renderer& renderer);

using namespace ECS;
Engine ecs = Engine();

int main(int argc, char** argv)
{
	ecs.RegisterComponent<Position>();
	ecs.RegisterComponent<Colour>();
	ecs.RegisterComponent<Extents>();
	ecs.RegisterComponent<Rect>();

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> colourDist(0, 255);

	float squareSize = 1;
	for (int i = 0; i < 800/squareSize; i++)
	{
		for (int j = 0; j < 800 / squareSize; j++)
		{
			EntityID test = ecs.Entity();

			Rect rect{
				{ squareSize / 2 + i * squareSize, squareSize / 2 + j * squareSize },
				{ squareSize, squareSize },
				{
					((800 / squareSize) - i) / (800 / squareSize) * 255,
					0,
					((800 / squareSize) - j) / (800 / squareSize) * 255,
					255
				}
			};
			ecs.AddComponent<Rect>(test, std::move(rect));
		}
	}

	ecs.RegisterSystem<Rect>()->Init(
		[](Rect& rect)
		{
			rect.colour.r -= 1;
		});

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
		renderer.SetClearColour({ 0, 0, 0, 255 });
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

static Uint64 accumulator = 0;

void Update()
{
	static Uint64 previousTime;
	Uint64 deltaTime = SDL_GetTicks64() - previousTime;

	Logger::Log("Delta Time: " + std::to_string(deltaTime));
	accumulator += deltaTime;
	if (accumulator >= 50)
	{
		ecs.RunSystems();
		accumulator -= 50;
	}

	previousTime = SDL_GetTicks64();
}

void Render(Renderer& renderer)
{
	renderer.Clear();

	ecs.GetSystem<Rect>().Each(
		[](Rect& rect) 
		{
			SDL2::GetRenderer()->DrawRect(rect.pos.value, rect.extents.value, rect.colour);
		});

	renderer.Present();
}