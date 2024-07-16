#include "Renderer.h"
#include <iostream>


Renderer::Renderer() :
	_rendererPtr{nullptr}
{
	_clearColour = { 0xFF, 0xFF, 0xFF, 0xFF };
}

Renderer::Renderer(const SDL2::WindowPtr& window, int index, Uint32 flags) :
	_rendererPtr{ SDL2::CreateRenderer(window, index, flags) }
{
	if (_rendererPtr == nullptr)
	{
		std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
	}
	_clearColour = { 0xFF, 0xFF, 0xFF, 0xFF };
}

bool Renderer::IsValid() const
{
	return _rendererPtr != nullptr;
}

void Renderer::Clear() const
{
	SetDrawColour(_clearColour);
	SDL_RenderClear(_rendererPtr.get());
}

void Renderer::Present() const
{
	SDL_RenderPresent(_rendererPtr.get());
}

void Renderer::SetClearColour(Colour colour)
{
	_clearColour = colour;
}

void Renderer::DrawTexture(FVector2 pos, SDL2::TexturePtr& texture, int width, int height)
{
	SDL_FRect dest = { pos.x, pos.y, width, height };
	SDL_RenderCopyExF(_rendererPtr.get(), texture.get(), nullptr, &dest, 0.0f, nullptr, SDL_FLIP_NONE);
}

void Renderer::DrawRect(FVector2 pos, FVector2 extents, Colour colour)
{
	SDL_FRect rect{ pos.x - extents.x/2, pos.y - extents.y/2, extents.x, extents.y };
	SetDrawColour(colour);
	SDL_RenderFillRectF(_rendererPtr.get(), &rect);
}

SDL2::TexturePtr Renderer::CreateTextureFromSurface(SDL2::SurfacePtr& surface)
{
	return SDL2::TexturePtr(SDL_CreateTextureFromSurface(_rendererPtr.get(), surface.get()));
}

void Renderer::SetDrawColour(Colour colour) const
{
	SDL_SetRenderDrawColor(_rendererPtr.get(), colour.r, colour.g, colour.b, colour.a);
}