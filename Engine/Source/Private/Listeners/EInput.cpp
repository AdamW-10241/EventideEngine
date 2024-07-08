#include "Listeners/EInput.h"
#include "EWindow.h"

EInput::EInput()
{
	OnKeyPressed = TMakeShared<EEvents<SDL_Scancode>>();
	OnKeyReleased = TMakeShared<EEvents<SDL_Scancode>>();
	OnMouseMoved = TMakeShared<EEvents<float, float, float, float>>();
	OnMouseScrolled = TMakeShared<EEvents<float>>();
	OnMousePressed = TMakeShared<EEvents<EUi8>>();
	OnMouseReleased = TMakeShared<EEvents<EUi8>>();

	m_lastMotion = SDL_MouseMotionEvent();
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
	bool mouseMoved = false;

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
		case SDL_MOUSEMOTION:
			// When the mouse moves run the mouse move event
			// This only runs when the mouse moves so we need another point...
			// ...to determine when the mouse has stopped motion
			OnMouseMoved->Run(
				static_cast<float>(e.motion.x),
				static_cast<float>(e.motion.y),
				static_cast<float>(e.motion.xrel),
				static_cast<float>(e.motion.yrel)
			);

			m_lastMotion = e.motion;
			mouseMoved = true;
			break;
		case SDL_MOUSEWHEEL:
			// Run the scoll event when the mouse is scrolled
			OnMouseScrolled->Run(e.wheel.preciseY);
			break;
		case SDL_MOUSEBUTTONDOWN:
			OnMousePressed->Run(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			OnMouseReleased->Run(e.button.button);
			break;
		}
	}

	// If the mouse hasn't moved and the last motion was movement
	// Run the mouse move function and 0 out the relative movement
	if (!mouseMoved && (m_lastMotion.xrel != 0 || m_lastMotion.yrel != 0)) {
		OnMouseMoved->Run(
			static_cast<float>(m_lastMotion.x),
			static_cast<float>(m_lastMotion.y),
			0.0f, 0.0f
		);

		m_lastMotion.xrel = 0;
		m_lastMotion.yrel = 0;
	}
}

void EInput::ShowCursor(const bool& enable)
{
	// Hide the mouse and set the cursor to the center of the screen if true
	// Using shorthand if statement to get result
	// [condition] ? [result if true] : [result if false]
	SDL_SetRelativeMouseMode(enable ? SDL_FALSE : SDL_TRUE);
}

bool EInput::IsCursorHidden() const
{
	return SDL_GetRelativeMouseMode() == SDL_TRUE ? true : false;
}
