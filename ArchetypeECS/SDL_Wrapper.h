#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <iostream>

class Renderer;
class Window;

namespace SDL2
{
	/// <summary>
	/// Deleters for SDL2 smart pointers
	/// </summary>
	struct Deleter
	{
		void operator()(SDL_Window* ptr) const { SDL_DestroyWindow(ptr); }
		void operator()(SDL_Renderer* ptr) const { SDL_DestroyRenderer(ptr); }
		void operator()(SDL_Texture* ptr) const { SDL_DestroyTexture(ptr); }
		void operator()(SDL_Surface* ptr) const { SDL_FreeSurface(ptr); }
	};
	typedef std::unique_ptr<SDL_Window, Deleter> WindowPtr;
	typedef std::unique_ptr<SDL_Renderer, Deleter> RendererPtr;
	typedef std::unique_ptr<SDL_Texture, Deleter> TexturePtr;
	typedef std::unique_ptr<SDL_Surface, Deleter> SurfacePtr;

	/// <summary>
	///  Create a window with the specified position, dimensions, and flags.
	///
	/// `flags` may be any of the following OR'd together:
	///
	/// -`SDL_WINDOW_FULLSCREEN`: fullscreen window
	/// -`SDL_WINDOW_FULLSCREEN_DESKTOP`: fullscreen window at desktop resolution
	/// -`SDL_WINDOW_OPENGL`: window usable with an OpenGL context
	/// -`SDL_WINDOW_VULKAN`: window usable with a Vulkan instance
	/// -`SDL_WINDOW_METAL`: window usable with a Metal instance
	/// -`SDL_WINDOW_HIDDEN`: window is not visible
	/// -`SDL_WINDOW_BORDERLESS`: no window decoration
	/// -`SDL_WINDOW_RESIZABLE`: window can be resized
	/// -`SDL_WINDOW_MINIMIZED`: window is minimized
	/// -`SDL_WINDOW_MAXIMIZED`: window is maximized
	/// -`SDL_WINDOW_INPUT_GRABBED`: window has grabbed input focus
	/// -`SDL_WINDOW_ALLOW_HIGHDPI`: window should be created in high - DPI mode if
	/// supported(>= SDL 2.0.1)
	///
	/// `SDL_WINDOW_SHOWN` is ignored by SDL_CreateWindow().The SDL_Window is
	/// implicitly shown if SDL_WINDOW_HIDDEN is not set. `SDL_WINDOW_SHOWN` may be
	/// queried later using SDL_GetWindowFlags().
	///
	/// On Apple's macOS, you //////must////// set the NSHighResolutionCapable Info.plist
	/// property to YES, otherwise you will not receive a High - DPI OpenGL canvas.
	///
	///If the window is created with the `SDL_WINDOW_ALLOW_HIGHDPI` flag, its size
	/// in pixels may differ from its size in screen coordinates on platforms with
	/// high - DPI support(e.g.iOS and macOS).Use SDL_GetWindowSize() to query the
	/// client area's size in screen coordinates, and SDL_GL_GetDrawableSize() or
	/// SDL_GetRendererOutputSize() to query the drawable size in pixels.Note that
	/// when this flag is set, the drawable size can vary after the window is
	/// created and should be queried after major window events such as when the
	/// window is resized or moved between displays.
	///
	/// If the window is set fullscreen, the width and height parameters `w` and
	///`h` will not be used.However, invalid size parameters(e.g.too large) may
	/// still fail.Window size is actually limited to 16384 x 16384 for all
	/// platforms at window creation.
	///
	/// If the window is created with any of the SDL_WINDOW_OPENGL or
	/// SDL_WINDOW_VULKAN flags, then the corresponding LoadLibrary function
	/// (SDL_GL_LoadLibrary or SDL_Vulkan_LoadLibrary) is called and the
	/// corresponding UnloadLibrary function is called by SDL_DestroyWindow().
	///
	/// If SDL_WINDOW_VULKAN is specified and there isn't a working Vulkan driver,
	/// SDL_CreateWindow() will fail because SDL_Vulkan_LoadLibrary() will fail.
	///
	/// If SDL_WINDOW_METAL is specified on an OS that does not support Metal,
	/// SDL_CreateWindow() will fail.
	///
	/// On non - Apple devices, SDL requires you to either not link to the Vulkan
	/// loader or link to a dynamic library version.This limitation may be removed
	/// in a future version of SDL.
	/// </summary>
	/// <param name="title">the title of the window, in UTF-8 encoding</param>
	/// <param name="x">the x position of the window, `SDL_WINDOWPOS_CENTERED`, or `SDL_WINDOWPOS_UNDEFINED`</param>
	/// <param name="y">the y position of the window, `SDL_WINDOWPOS_CENTERED`, or `SDL_WINDOWPOS_UNDEFINED`</param>
	/// <param name="w">the width of the window, in screen coordinates</param>
	/// <param name="h">the height of the window, in screen coordinates</param>
	/// <param name="flags">0, or one or more SDL_WindowFlags OR'd together</param>
	/// <returns></returns>
	WindowPtr CreateWindow(const char* title,
		int x, int y, int w,
		int h, Uint32 flags);

	/// <summary>
	/// Create a 2D rendering context for a window.
	/// </summary>
	/// <param name="window">the window where rendering is displayed</param>
	/// <param name="index">the index of the rendering driver to initialize, or -1 to
	///			initialize the first one supporting the requested flags</param>
	/// <param name="flags">0, or one or more SDL_RendererFlags OR'd together</param>
	/// <returns>a valid rendering context or NULL if there was an error; call
	///			SDL_GetError() for more information.</returns>
	RendererPtr CreateRenderer(const WindowPtr& window, int index, Uint32 flags);

	/// <summary>
	/// Given a path loads image and creates surface from it
	/// </summary>
	/// <param name="path">Path of image</param>
	/// <returns>Smart pointer to surface  containing image</returns>
	SurfacePtr LoadImage(const char* path);

	/// <summary>
	/// Initialises SDL
	/// </summary>
	/// <returns>Whether it correctly initialised</returns>
	bool Init();

	/// <summary>
	/// Close down SDL
	/// </summary>
	void Quit();

	/// <summary>
	/// Stores pointers to renderer and window for requesting
	/// </summary>
	void StorePointers(Renderer* renderer, Window* window);

	Renderer* GetRenderer();
	Window* GetWindow();
}