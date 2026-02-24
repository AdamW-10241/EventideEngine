#include "Game/GameObjects/CustomObjects/GUIButton.h"
#include "GameInput.h"
#include "Graphics/EGraphicsEngine.h"

void GUIButton::OnRegisterInputs(const TShared<EInput>& m_input)
{
	// Mouse motion
	m_input->OnMousePressed->Bind([this, m_input](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			// Get button transform and compare if cursor on button
			if (IsMouseOnButton(m_input)) {
				OnButtonPressed();
			}
		}
	});
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
