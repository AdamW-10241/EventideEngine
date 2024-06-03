#include "EWindow.h"
#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"

// External Libs
#include <SDL/SDL.h>

EWindow::EWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;

	EDebug::Log("Window created.");
}

EWindow::~EWindow()
{
	// If the SDL window exists, destroy it
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	EDebug::Log("Window destroyed.");
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
		EDebug::Log(
			"SDL failed to create window: " + std::string(SDL_GetError()), 
			LT_ERROR);
		CloseWindow();
		return false;
	}

	// Create the graphics engine objects
	m_graphicsEngine = std::make_unique<EGraphicsEngine>();

	// Initialise the graphics engine and test if it failed
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		EDebug::Log("Window failed to initialise Graphics Engine.", LT_ERROR);
		m_graphicsEngine = nullptr;
		return false;
	}

	return true;
}

void EWindow::Render()
{
	// Render the graphics engine if exists
	if (m_graphicsEngine) {
		m_graphicsEngine->Render(m_sdlWindow);
	}
}
