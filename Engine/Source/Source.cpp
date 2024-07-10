#include "EngineTypes.h"

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "EWindow.h"
#include "Listeners/EInput.h"
#include "Listeners/EEvents.h"
#include "Graphics/ESCamera.h"

// Source Variables
TShared<EWindow> m_window;
TShared<EInput> m_input;

// Source Functions
bool Initialise() {
	// Initialise the components of SDL that we need
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Tell SDL that we will be rendering in OpenGL version 460 or 4.60
	// 4 is the major version
	// .60 is the minor version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Make sure compativbility mode is run for an OpenGL version not in SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	// Set the bit depth for each colour
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	// Creating the window object
	m_window = TMakeShared<EWindow>();

	// Creating an SDL window
	if (!m_window->CreateWindow({ "Game Window", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		720, 720 })) {
		return false;
	}

	// Create the input class and assign the window
	m_input = TMakeShared<EInput>();
	m_input->InitInput(m_window);

	return true;
}

void Cleanup() {
	// Exit SDL and deallocate all initialised SDL components
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	// Initialise the engine
	if (!Initialise()) {
		Cleanup();
		return -1;
	}

	// Register the window inputs
	m_window->RegisterInput(m_input);

	// Keep the game open as long as the window is open
	while (!m_window->IsPendingClose()) {
		// Handle inputs
		m_input->UpdateInputs();

		// Update the window
		m_window->Update();

		// Render the window
		m_window->Render();
	}

	// Cleanup the engine
	Cleanup();
	return 0;
}