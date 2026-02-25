#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIButton : public EScreenObject {
public:
	using EScreenObject::EScreenObject;

	// Lamba bindings
	std::function<void()> OnStarted;
	std::function<void()> OnTicked;
	std::function<void()> OnPostTicked;
	std::function<void()> OnPressed;
	std::function<void()> OnHeld;
	std::function<void()> OnReleased;

	// Extend lambda bindings
	void ExtendBinding(std::function<void()>& target, std::function<void()> func) {
		auto existing = target;
		target = [existing, func]() {
			if (existing) existing();
			if (func) func();
		};
	}

	// Set pressed color
	void SetPressedColor(const glm::vec4& pressed, const glm::vec4& released = glm::vec4(1.0f)) {
		auto weak = GetWeakRef<GUIButton>();

		// Set color on pressed
		ExtendBinding(OnPressed, [weak, pressed]() {
			if (const auto& btn = weak.lock()) btn->GetSprite(0).lock()->GetRenderColor() = pressed;
		});
		// Set color on released
		ExtendBinding(OnReleased, [weak, released]() {
			if (const auto& btn = weak.lock()) btn->GetSprite(0).lock()->GetRenderColor() = released;
		});
	}

protected:
	virtual void OnStart() override {
		EScreenObject::OnStart();
		if (OnStarted) OnStarted();
	}

	virtual void OnRegisterInputs(const TShared<EInput>& m_input) override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnPostTick(float deltaTime) override;

	virtual void OnButtonPressed() { 
		if (OnPressed) OnPressed(); 
	}

	virtual void OnButtonReleased() {
		if (OnReleased) OnReleased();
	}

protected:
	// Store state
	bool m_buttonHeld = false;

private:
	const bool IsMouseOnButton(const TShared<EInput>& m_input);
};