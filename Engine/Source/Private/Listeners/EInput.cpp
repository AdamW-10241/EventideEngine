#include "Listeners/EInput.h"
#include "EWindow.h"

EInput::EInput()
{
	OnKeyPressed = TMakeShared<EEvents<SDL_Scancode>>();
	OnKeyReleased = TMakeShared<EEvents<SDL_Scancode>>();
	OnMousePressed = TMakeShared<EEvents<>>();
	OnMousePressed = TMakeShared<EEvents<>>();
}

void EInput::InitInput(const TShared<EWindow>& window)
{
	// To assign a weak pointer, just plug in shared pointer
	m_window = window;
}

void EInput::UpdateInputs()
{
	// Poll and handle input events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		// Quit program
		if (e.type == SDL_QUIT) {
			if (const auto& windowRef = m_window.lock()) {
				windowRef->CloseWindow();
			}
		}

		// Run different events based on type of input
		switch (e.type) {
		case SDL_KEYDOWN:
			// Check if this key has repeated since being held down
			if (e.key.repeat == 0)
				OnKeyPressed->Run(e.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			// Check if this key has repeated since being held down
			if (e.key.repeat == 0)
				OnKeyReleased->Run(e.key.keysym.scancode);
			break;
		default:
			break;
		}
	}
}
