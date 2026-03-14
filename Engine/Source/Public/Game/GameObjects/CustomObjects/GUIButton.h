#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIButton : public EScreenObject {
public:
	using EScreenObject::EScreenObject;

	float GetTimeHeld() const { return m_timeHeld; }

	// Button Lambda bindings
	std::function<void()> OnPressed;
	std::function<void(float, float)> OnHeld;
	std::function<void()> OnReleased;

	// Set pressed colors
	void SetSpritePressedColor(const glm::vec4 pressed, std::optional<glm::vec4> released = std::nullopt) {
		SetSpritePressedColor(0, pressed, released);
	}
	void SetSpritePressedColor(const int index, const glm::vec4 pressed, std::optional<glm::vec4> released = std::nullopt) {
		// Validate sprite
		const auto& sprite = GetSprite(index);
		if (!sprite.lock()) {
			EDebug::Log("GUIButton - Sprite at index " + toEString(index) + " does not exist.", LT_ERROR);
			return;
		}

		// Set color on pressed
		BindObjectEvent(&GUIButton::OnPressed, GetSprite(index), [pressed](const TShared<GUIButton>& btn, const TShared<ESprite>& spr) {
			spr->GetRenderColor() = pressed;
		});
		// Set color on released
		glm::vec4 releasedColor = released.has_value() ? released.value() : GetSprite(index).lock()->GetRenderColor();
		BindObjectEvent(&GUIButton::OnReleased, GetSprite(index), [releasedColor](const TShared<GUIButton>& btn, const TShared<ESprite>& spr) {
			spr->GetRenderColor() = releasedColor;
		});
	}

protected:
	virtual void OnRegisterInputs(const TShared<EInput>& m_input) override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnButtonPressed() {
		if (OnPressed) OnPressed();
	}

	virtual void OnButtonHeld(float deltaTime);

	virtual void OnButtonReleased() {
		if (OnReleased) OnReleased();
	}

protected:
	// Store state
	TWeak<EInput> m_inputWeak;

	bool m_buttonHeld = false;

	float m_timeHeld = 0.0f;

	TArray<EUi8> m_inputBindings;

private:
	const bool IsMouseOnButton(const TShared<EInput>& input);
};