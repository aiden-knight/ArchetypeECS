#include "Renderer.h"
#include "Window.h"
#include "JSONLoader.h"

#include "Engine.h"

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

	struct Health
	{
		int value = 100;
	};
	struct Mana
	{
		int value = 0;
	};
	ecs.RegisterComponent<Mana>();
	ecs.RegisterComponent<Health>();
	ecs.RegisterComponent<float>();

	// create two entities
	EntityID entity = ecs.Entity();
	EntityID second = ecs.Entity();

	Logger::Log("EntityID (entity): " + std::to_string(entity));
	Logger::Log("EntityID (second): " + std::to_string(entity));
	Logger::Break();

	// add health components
	ecs.AddComponent<Health>(entity, {42});
	ecs.AddComponent<Health>(second, {13});

	Logger::Log("Health value before adding mana: " + std::to_string(ecs.GetComponent<Health>(entity)->value));
	Logger::Break();

	// add mana component to only first
	ecs.AddComponent<Mana>(entity, {-20});


	Logger::Log("Health value after adding mana: " + std::to_string(ecs.GetComponent<Health>(entity)->value));
	Logger::Log("Second health value after adding mana: " + std::to_string(ecs.GetComponent<Health>(second)->value));

	Logger::Log("Mana value: " + std::to_string(ecs.GetComponent<Mana>(entity)->value));
	ecs.AddComponent<float>(entity, { 3.14f });

	Logger::Break();
	Logger::Log("Float value: " + std::to_string(*ecs.GetComponent<float>(entity)));

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