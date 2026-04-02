#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIButton : public EScreenObject {
public:
	using EScreenObject::EScreenObject;

	// Button Lambda bindings
	std::function<void()> OnPressed;
	std::function<void(float, float)> OnHeld;
	std::function<void()> OnReleased;

	float GetTimeHeld() const { return m_timeHeld; }

	// Set pressed colors
	void SetSpritePressedColor(const glm::vec4 pressed, std::optional<glm::vec4> released = std::nullopt) {
		SetSpritePressedColor(0, pressed, released);
	}
	void SetSpritePressedColor(const int index, const glm::vec4 pressed, std::optional<glm::vec4> released = std::nullopt) {
		// Validate sprite
		auto spriteRef = GetSprite(index);
		if (!spriteRef.lock()) {
			EDebug::Log("GUIButton - Sprite at index " + toEString(index) + " does not exist.", LT_ERROR);
			return;
		}

		// Set colors
		BIND_EVENT_EXT(GetSharedRef<GUIButton>(), OnPressed, GetSprite(index), [pressed](const TShared<GUIButton>& obj, const TShared<ESprite>& spr) {
			spr->GetRenderColor() = pressed;
		});
		glm::vec4 releasedColor = released.has_value() ? released.value() : GetSprite(index).lock()->GetRenderColor();
		BIND_EVENT_EXT(GetSharedRef<GUIButton>(), OnReleased, GetSprite(index), [releasedColor](const TShared<GUIButton>& obj, const TShared<ESprite>& spr) {
			spr->GetRenderColor() = releasedColor;
		});
	}

	// Add scaling for pressing button
	void AddPressAndReleaseScaling(float pressScale = 0.9f, float releaseScale = 1.0f) {
		BIND_EVENT_SELF(GetSharedRef<GUIButton>(), OnPressed, [pressScale](const TShared<GUIButton>& btn) {
			btn->SetSpritesRenderScales(pressScale);
		});
		BIND_EVENT_SELF(GetSharedRef<GUIButton>(), OnReleased, [releaseScale](const TShared<GUIButton>& btn) {
			btn->SetSpritesRenderScales(releaseScale);
		});
	}

	// Set sprite anchor based on mouse
	void SetSpriteAnchorMouse(int index = 0);

	// Get if button is held
	bool GetButtonHeld() const { return m_buttonHeld; }

	// Set if held until released
	void SetHeldUntilReleased(bool heldUntilReleased = true) { m_heldUntilReleased = heldUntilReleased; }

	// Get grab offset
	glm::vec2 GetGrabOffset() const { return m_grabOffset; }

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

	TArray<EUi8> m_inputBindings;

	bool m_buttonHeld = false;

	float m_timeHeld = 0.0f;

	bool m_heldUntilReleased = false;

	glm::vec2 m_grabOffset = glm::vec2(0.0f);

private:
	const bool IsMouseOnButton(const TShared<EInput>& input);
};