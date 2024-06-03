// System Libs
#include <iostream>

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "EWindow.h"

// Source Variables
std::unique_ptr<EWindow> m_window;

// Source Functions
bool Initialise() {
	// Initialise the components of SDL that we need
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Creating the window object
	m_window = std::make_unique<EWindow>();

	// Creating an SDL window
	if (!m_window->CreateWindow({ "Game Window", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720 })) {
		return false;
	}

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

	// Keep the game open as long as the window is open
	while (!m_window->IsPendingClose()) {
		// TODO: Game Loop
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				m_window->CloseWindow();
		}

		// Render the window
		m_window->Render();
	}

	// Cleanup the engine
	Cleanup();
	return 0;
}