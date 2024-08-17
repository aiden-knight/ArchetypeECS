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

using namespace ECS;
Engine ecs = Engine();
char* TextureBuffer;
SDL_Surface* surface;

int windowHeight;
int windowWidth;

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
		windowHeight = data.height;
		windowWidth = data.width;
		TextureBuffer = new char[3 * windowWidth * windowHeight];
		surface = SDL_CreateRGBSurfaceFrom(TextureBuffer, windowWidth, windowHeight, 24, windowWidth * 3, 0, 0, 0, 0);
		// store pointers to renderer and window in case components need them
		SDL2::StorePointers(&renderer, &window);
		renderer.SetClearColour({ 0, 0, 0, 255 });

		ecs.RegisterComponent<Pixel>(windowWidth * windowHeight);

		uint8_t threadNumber = 4;
		ecs.RegisterSystem<Pixel>(threadNumber)->Init(
			[](Pixel& pixel)
			{
				pixel.colour.r -= 1;
				pixel.colour.b -= 1;
			});
		ecs.RegisterSystem<Pixel>(threadNumber)->Init(
			[](Pixel& pixel)
			{
				FVector2& fpos = pixel.pos.value;
				int pos = 3 * (fpos.y * windowWidth + fpos.x);

				TextureBuffer[pos + 0] = pixel.colour.r;
				TextureBuffer[pos + 1] = pixel.colour.g;
				TextureBuffer[pos + 2] = pixel.colour.b;
			});

		for (float x = 0; x < windowWidth; x++)
		{
			for (float y = 0; y < windowHeight; y++)
			{
				EntityID test = ecs.Entity();

				Pixel pixel{
					{ x, y },
					{
						((windowWidth - x) / windowWidth) * 255,
						0,
						((windowHeight - y) / windowHeight) * 255,
						255
					}
				};
				ecs.AddComponent<Pixel>(test, std::move(pixel));
			}
		}
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
	SDL_FreeSurface(surface);
	delete[] TextureBuffer;
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
	static Uint64 previousTime = SDL_GetTicks64();
	Uint64 deltaTime = SDL_GetTicks64() - previousTime;
	previousTime = SDL_GetTicks64();

	Logger::Log("deltaTime: " + std::to_string(deltaTime));
	ecs.RunSystems();

}

void Render(Renderer& renderer)
{
	SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(SDL2::GetWindow()->_windowPtr.get()), NULL);
	SDL_UpdateWindowSurface(SDL2::GetWindow()->_windowPtr.get());
}