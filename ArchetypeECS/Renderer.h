#pragma once
#include "SDL_Wrapper.h"
#include "Structures.h"

/// <summary>
/// Class to wrap the render calls in
/// </summary>
class Renderer
{
public:
	Renderer();
	Renderer(const SDL2::WindowPtr& window, int index, Uint32 flags);

	/// <summary>
	/// Returns whether the SDL_Renderer is valid
	/// </summary>
	/// <returns></returns>
	bool IsValid() const;

	/// <summary>
	/// Clear back buffer with _clearColour
	/// </summary>
	void Clear() const;

	/// <summary>
	/// Present back buffer
	/// </summary>
	void Present() const;

	/// <summary>
	/// Sets clear colour
	/// </summary>
	/// <param name="colour">Colour to set to</param>
	void SetClearColour(Colour colour);

	/// <summary>
	/// Draws texture to location on screen
	/// </summary>
	/// <param name="pos">position to draw to</param>
	/// <param name="texture">texture to draw</param>
	/// <param name="width">width of texture</param>
	/// <param name="height">height of texture</param>
	void DrawTexture(FVector2 pos, SDL2::TexturePtr& texture, int width, int height);
	void DrawTexture(SDL2::TexturePtr& texture);
	void DrawRect(FVector2 pos, FVector2 extents, Colour colour);
	void DrawPoint(FVector2 pos, Colour colour);

	/// <summary>
	/// Creates texture from given surface
	/// </summary>
	SDL2::TexturePtr CreateTextureFromSurface(SDL2::SurfacePtr& surface);

	SDL2::TexturePtr CreateTexture(Uint32 format, int access, int w, int h);

	void SetRenderTarget(SDL2::TexturePtr& texture);
	void ClearRenderTarget();

	/// <summary>
	/// Sets current render draw colour
	/// </summary>
	/// <param name="colour">Colour to set to</param>
	void SetDrawColour(Colour colour) const;

private:
	SDL2::RendererPtr _rendererPtr; // can't be const or breaks move operator
	Colour _clearColour;
};
