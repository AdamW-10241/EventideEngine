#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIButton : public EScreenObject {
public:
	using EScreenObject::EScreenObject;

	// Lamba bindings
	std::function<void()> OnStarted;
	std::function<void(float)> OnTicked;
	std::function<void(float)> OnPostTicked;
	std::function<void()> OnPressed;
	std::function<void(float, float)> OnHeld;
	std::function<void()> OnReleased;

	// Extend lambda bindings
	// Adds new binding to existing lambda passing args
	template<typename... Args, typename Func>
	void ExtendBinding(std::function<void(Args...)> GUIButton::* target, Func func) {
		auto existing = this->*target;
		this->*target = [existing, func](Args... args) {
			if (existing) existing(args...);
			if constexpr (std::is_invocable_v<Func, Args...>) {
				func(args...);
			}
		};
	}

	// Set pressed color
	void SetPressedColor(const glm::vec4& pressed, const glm::vec4& released = glm::vec4(1.0f)) {
		auto weak = GetWeakRef<GUIButton>();

		// Set color on pressed
		ExtendBinding(&GUIButton::OnPressed, [weak, pressed]() {
			if (const auto& btn = weak.lock()) btn->GetSprite(0).lock()->GetRenderColor() = pressed;
		});
		// Set color on released
		ExtendBinding(&GUIButton::OnReleased, [weak, released]() {
			if (const auto& btn = weak.lock()) btn->GetSprite(0).lock()->GetRenderColor() = released;
		});
	}

	float GetTimeHeld() const { return m_timeHeld; }

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

	virtual void OnButtonHeld(float deltaTime);

	virtual void OnButtonReleased() {
		if (OnReleased) OnReleased();
	}

protected:
	// Store state
	TWeak<EInput> m_inputWeak;

	bool m_buttonHeld = false;

	float m_timeHeld = 0.0f;

private:
	const bool IsMouseOnButton(const TShared<EInput>& input);
};