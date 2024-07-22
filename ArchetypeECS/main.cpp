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

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> colourDist(0, 255);
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			EntityID test = ecs.Entity();
			ecs.AddComponent<Position>(test, { 25 + i*50.f, 25 + j*50.f });
			ecs.AddComponent<Extents>(test, { 50, 50 });

			Colour colour;
			colour.r = colourDist(rng);
			colour.g = colourDist(rng);
			colour.b = colourDist(rng);
			colour.a = 255;
			ecs.AddComponent<Colour>(test, std::move(colour));
		}
	}

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

void Update()
{
	static Uint64 previousTime;
	Uint64 deltaTime = SDL_GetTicks64() - previousTime;



	previousTime = SDL_GetTicks64();
}

void Render(Renderer& renderer)
{
	renderer.Clear();

	ecs.GetSystem<Position, Extents, Colour>().Each(
		[](Position& p, Extents& e, Colour& c) 
		{
			SDL2::GetRenderer()->DrawRect(p.value, e.value, c);
		});

	renderer.Present();
}