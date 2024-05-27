#include "EWindow.h"

// External Libs
#include <SDL/SDL.h>

EWindow::EWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;

	std::cout << "Window created." << std::endl;
}

EWindow::~EWindow()
{
	// If the SDL window exists, destroy it
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	std::cout << "Window destroyed." << std::endl;
}

bool EWindow::CreateWindow(const ESWindowParams& params)
{
	// Enabling OpenGL in our SDL window
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	// Assigning the parameters to the member for the window
	m_params = params;

	// Add vsync flag if selected
	if (m_params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	// Add fullscreen flag if selected
	if (m_params.fullscreen)
		// Fullscreen borderless
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		// Windowed mode
		windowFlags += SDL_WINDOW_SHOWN;

	// Create the SDL window
	m_sdlWindow = SDL_CreateWindow(
		m_params.title.c_str(),
		m_params.x,
		m_params.y,
		m_params.w,
		m_params.h,
		windowFlags
	);
	
	// Check if SDL window was created
	if (!m_sdlWindow) {
		std::cout << "SDL failed to create window: " << SDL_GetError() << std::endl;
		CloseWindow();
		return false;
	}

	return true;
}
