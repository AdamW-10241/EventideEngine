#include "Game/GameObjects/CustomObjects/GUIButton.h"
#include "GameInput.h"
#include "Graphics/EGraphicsEngine.h"

void GUIButton::OnRegisterInputs(const TShared<EInput>& m_input)
{
	// Bind mouse events
	m_input->OnMousePressed->Bind([this, m_input](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT && IsMouseOnButton(m_input)) {
			// Button pressed
			m_buttonHeld = true;
			OnButtonPressed();
		}
	});

	m_input->OnMouseReleased->Bind([this, m_input](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT && m_buttonHeld) {
			// Button released
			m_buttonHeld = false;
			OnButtonReleased();
		}
	});
}

void GUIButton::OnTick(float deltaTime)
{
	// Run lambdas
	if (OnTicked) OnTicked();

	if (m_buttonHeld) OnHeld();
}

void GUIButton::OnPostTick(float deltaTime)
{
	// Run lambdas
	if (OnPostTicked) OnPostTicked();
}

const bool GUIButton::IsMouseOnButton(const TShared<EInput>& m_input)
{
	// Get mouse location
	SDL_MouseMotionEvent& mouseLastMotion = m_input->GetMouseLastMotion();

	// Get button location
	ESTransform2D buttonTransform = GetSprite(0).lock()->GetTransform();
	float left = buttonTransform.position.x;
	float top = buttonTransform.position.y;
	float right = buttonTransform.position.x + buttonTransform.scale.x;
	float bottom = buttonTransform.position.y + buttonTransform.scale.y;

	// Return whether mouse is inbetween sides of button
	return (mouseLastMotion.x > left && mouseLastMotion.y > top &&
		mouseLastMotion.x < right && mouseLastMotion.y < bottom);
}
