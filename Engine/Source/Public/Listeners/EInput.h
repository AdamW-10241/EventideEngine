#pragma once
#include "EngineTypes.h"
#include "Listeners/EEvents.h"

// External Libs
#include <SDL/SDL_keycode.h>
#include <SDL/SDL_events.h>

class EWindow;

class EInput {
public:
	EInput();
	
	// Initialising the window
	void InitInput(const TShared<EWindow>& window);
	
	// Updating the inputs
	void UpdateInputs();

	// On a press event
	TShared<EEvents<SDL_Scancode>> OnKeyPressed;

	// On a release event
	TShared<EEvents<SDL_Scancode>> OnKeyReleased;

	// On a press event
	TShared<EEvents<>> OnMousePressed;

	// On a release event
	TShared<EEvents<>> OnMouseReleased;

private:
	// Weak pointer to the window to not count as a reference
	// so that the inout does not control if the window gets destroyed
	TWeak<EWindow> m_window;
};