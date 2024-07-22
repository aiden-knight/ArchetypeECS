#include "Renderer.h"
#include "Window.h"
#include "JSONLoader.h"

#include "Engine.h"
#include "System.h"

/// <summary>
/// Contains the loop for polling SDL events
/// </summary>
/// <returns>True if should quit</returns>
bool PollEvents();

void Update();
void Render(Renderer& renderer);

int main(int argc, char** argv)
{
	using namespace ECS;
	Engine ecs = Engine();

	ecs.RegisterComponent<Mana>();
	ecs.RegisterComponent<Health>();

	// create two entities
	EntityID first = ecs.Entity();
	EntityID second = ecs.Entity();
	EntityID third = ecs.Entity();

	ecs.AddComponent<Health>(3);
	ecs.AddComponent<float>(first);
	Logger::Break();

	// add health components
	ecs.AddComponent<Health>(first, { 42 });
	ecs.AddComponent<Health>(second, { 13 });
	ecs.AddComponent<Health>(third, { 3 });

	Logger::Log("Entity 1 health value before adding mana: " + std::to_string(ecs.GetComponent<Health>(first)->value));
	Logger::Log("Entity 2 health value before adding mana: " + std::to_string(ecs.GetComponent<Health>(second)->value));
	Logger::Log("Entity 3 health value before adding mana: " + std::to_string(ecs.GetComponent<Health>(third)->value));

	Logger::Break();

	// add mana component to first and third
	ecs.AddComponent<Mana>(first, { -20 });
	ecs.AddComponent<Mana>(third);

	Logger::Log("Entity 1 health value after adding mana to entity 1 and 3: " + std::to_string(ecs.GetComponent<Health>(first)->value));
	Logger::Log("Entity 2 health value after adding mana to entity 1 and 3: " + std::to_string(ecs.GetComponent<Health>(second)->value));
	Logger::Log("Entity 3 health value after adding mana to entity 1 and 3: " + std::to_string(ecs.GetComponent<Health>(third)->value));
	Logger::Break();

	Logger::Log("Entity 1 mana value: " + std::to_string(ecs.GetComponent<Mana>(first)->value));
	if (Mana* mana = ecs.GetComponent<Mana>(second))
	{
		Logger::Log("Entity 2 mana value: " + std::to_string(mana->value));
	}
	else
	{
		Logger::Log("Entity 2 mana was nullptr");
	}
	
	Logger::Log("Entity 3 mana value: " + std::to_string(ecs.GetComponent<Mana>(third)->value));
	Logger::Break();

	ecs.RegisterSystem<Health>()->Init(
		[](Health& h)
		{
			Logger::Log("Print System: " + std::to_string(h.value));
		});

	ecs.RunSystems();
	Logger::Break();

	// create system for health components
	auto healthSystem = ecs.GetSystem<Health>();

	// double all health values
	healthSystem.Each(
		[](Health& h)
		{
			h.value *= 2;
		});

	// print all health values

	ecs.RunSystems();
	Logger::Break();

	ecs.GetSystem<Health, Mana>().Each([](Health& h, Mana& m)
		{
			Logger::Log("Has both health and mana (Health: " + std::to_string(h.value) + ", Mana: " + std::to_string(m.value) + ")");
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