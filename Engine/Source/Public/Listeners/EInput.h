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

	// On mouse moved
	// @param 1: mouse position x
	// @param 2: mouse position y
	// @param 3: x relative to last x
	// @param 4: y relative to last y
	TShared<EEvents<float, float, float, float>> OnMouseMoved;

	// When the mouse scrolls, pass the delta
	TShared<EEvents<float>> OnMouseScrolled;

	// Listen for mouse pressed events
	TShared<EEvents<EUi8>> OnMousePressed;

	// Listen for mouse released events
	TShared<EEvents<EUi8>> OnMouseReleased;

	// Hide mouse
	// This will also make sure the mouse is centered to the screen when moving
	void ShowCursor(const bool& enable);

	// Get the state of the cursor
	bool IsCursorHidden() const;

private:
	// Weak pointer to the window to not count as a reference
	// so that the inout does not control if the window gets destroyed
	TWeak<EWindow> m_window;

	// Last mouse motion event
	SDL_MouseMotionEvent m_lastMotion;
};