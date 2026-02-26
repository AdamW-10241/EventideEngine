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
	// Extends a binding
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

	// Extends a binding with automatic weak pointer safety for passed object
	template<typename T, typename Func, typename... Args>
	void BindObjectEvent(std::function<void(Args...)> GUIButton::* target, TWeak<T> weak, Func action) {
		ExtendBinding(target, [weak, action](Args... args) {
			if (const auto& obj = weak.lock()) action(obj, args...);
		});
	}

	// Set pressed color
	void SetPressedColor(const glm::vec4& pressed, const glm::vec4& released = glm::vec4(1.0f)) {
		SetPressedColor(0, pressed, released);
	}
	void SetPressedColor(const int index, const glm::vec4& pressed, const glm::vec4& released = glm::vec4(1.0f)) {
		// Set color on pressed
		BindObjectEvent(&GUIButton::OnPressed, GetSprite(index), [pressed](const TShared<ESprite>& spr) {
			spr->GetRenderColor() = pressed;
		});
		// Set color on released
		BindObjectEvent(&GUIButton::OnReleased, GetSprite(index), [released](const TShared<ESprite>& spr) {
			spr->GetRenderColor() = released;
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

	TArray<EUi8> m_inputBindings;

private:
	const bool IsMouseOnButton(const TShared<EInput>& input);
};