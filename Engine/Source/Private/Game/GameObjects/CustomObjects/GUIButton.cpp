#include "Game/GameObjects/CustomObjects/GUIButton.h"
#include "GameInput.h"
#include "Graphics/EGraphicsEngine.h"

void GUIButton::OnRegisterInputs(const TShared<EInput>& m_input)
{
	m_inputWeak = m_input;

	// Bind mouse events
	SetInputBinding(m_input, &EInput::OnMousePressed, [this, m_input](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT && IsMouseOnButton(m_input)) {
			// Button pressed
			m_buttonHeld = true;
			m_timeHeld = 0.0f;
			OnButtonPressed();
		}
	});

	SetInputBinding(m_input, &EInput::OnMouseReleased, [this, m_input](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT && m_buttonHeld) {
			// Button released
			m_buttonHeld = false;
			m_timeHeld = 0.0f;
			OnButtonReleased();
		}
	});
}

void GUIButton::OnTick(float deltaTime)
{
	// Run lambdas
	if (OnTicked) OnTicked(deltaTime);

	if (m_buttonHeld) OnButtonHeld(deltaTime);
}

void GUIButton::OnPostTick(float deltaTime)
{
	// Run lambdas
	if (OnPostTicked) OnPostTicked(deltaTime);
}

void GUIButton::OnButtonHeld(float deltaTime)
{
	// Check still held
	if (const auto& inputRef = m_inputWeak.lock()) {
		if (!IsMouseOnButton(inputRef)) {
			m_buttonHeld = false;
			m_timeHeld = 0.0f;
			OnButtonReleased();
			return;
		}
	}
	
	// Update time held
	m_timeHeld += deltaTime;
	
	// Run lambdas
	if (OnHeld) OnHeld(deltaTime, m_timeHeld);
}

const bool GUIButton::IsMouseOnButton(const TShared<EInput>& m_input)
{
	// Get positions
	SDL_MouseMotionEvent& mouseLastMotion = m_input->GetMouseLastMotion();
	ESTransform2D buttonTransform;
	if (const auto& spriteRef = GetSprite(0).lock()) {
		buttonTransform = spriteRef->GetTransform();
	}

	// Get mouse position relative to button center
	glm::vec2 center = buttonTransform.position + buttonTransform.scale * 0.5f;
	glm::vec2 mousePos = glm::vec2(mouseLastMotion.x, mouseLastMotion.y) - center;

	// Rotate mouse position by negative rotation to bring it into local space
	float angle = glm::radians(-buttonTransform.rotation);
	glm::vec2 localMouse = glm::vec2(
		mousePos.x * cos(angle) - mousePos.y * sin(angle),
		mousePos.x * sin(angle) + mousePos.y * cos(angle)
	);

	// Now check against unrotated half extents
	glm::vec2 halfScale = buttonTransform.scale * 0.5f;
	bool isMouseOnButton = (localMouse.x > -halfScale.x && localMouse.x < halfScale.x &&
		localMouse.y > -halfScale.y && localMouse.y < halfScale.y);
	return isMouseOnButton;
}
