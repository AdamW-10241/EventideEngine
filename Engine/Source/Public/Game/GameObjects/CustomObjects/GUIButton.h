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

	float GetTimeHeld() const { return m_timeHeld; }

	// Extend lambda bindings
	// Extends a binding
	template<typename... Args, typename Func>
	void ExtendBinding(std::function<void(Args...)> GUIButton::* target, Func func) {
		auto existing = this->*target;
		this->*target = [existing, func](Args... args) {
			if (existing) existing(args...);
			func(args...);
		};
	}

	// Extends a binding with weak safety on 'this' only (no external object)
	template<typename Func, typename... Args>
	void BindSelfEvent(std::function<void(Args...)> GUIButton::* target, Func action) {
		static_assert(
			std::is_invocable_v<Func, TShared<GUIButton>, Args...>,
			"GUIButton - BindSelfEvent: action must take (TShared<GUIButton>, ...Args)"
			);
		auto self = GetWeakRef<GUIButton>();
		ExtendBinding(target, [self, action](Args... args) {
			if (const auto& s = self.lock())
				if constexpr (std::is_invocable_v<Func, TShared<GUIButton>, Args...>)
					action(s, args...);
			});
	}

	// Extends a binding with weak safety on 'this' and external object
	template<typename T, typename Func, typename... Args>
	void BindObjectEvent(std::function<void(Args...)> GUIButton::* target, TWeak<T> weak, Func action) {
		static_assert(
			std::is_invocable_v<Func, TShared<GUIButton>, TShared<T>, Args...>,
			"GUIButton - BindObjectEvent: action must take (TShared<GUIButton>, TShared<T>, ...Args)"
			);
		auto self = GetWeakRef<GUIButton>();
		ExtendBinding(target, [self, weak, action](Args... args) {
			if (const auto& s = self.lock()) {
				if (const auto& obj = weak.lock())
					if constexpr (std::is_invocable_v<Func, TShared<GUIButton>, TShared<T>, Args...>)
						action(s, obj, args...);
			}
		});
	}

	#define BTN_BIND_EVENT_RAW(obj, event, action) \
		obj->ExtendBinding(&std::remove_reference_t<decltype(*obj)>::event, action)
	#define BTN_BIND_EVENT_SELF(obj, event, action) \
		obj->BindSelfEvent(&std::remove_reference_t<decltype(*obj)>::event, action)
	#define BTN_BIND_EVENT_EXT(obj, event, ...) \
		obj->BindObjectEvent(&std::remove_reference_t<decltype(*obj)>::event, __VA_ARGS__)

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

	//void SetSpritesPressedColors(const glm::vec4 pressed, const glm::vec4 released = glm::vec4(1.0f)) {
	//	// Set color on pressed
	//	BindSelfEvent(&GUIButton::OnPressed, [pressed](const TShared<GUIButton>& btn) {
	//		for (auto spr : btn->GetSprites()) {
	//			spr.lock()->SetRenderColor(pressed);
	//		}
	//	});
	//	// Set color on released
	//	BindSelfEvent(&GUIButton::OnReleased, [released](const TShared<GUIButton>& btn) {
	//		for (auto spr : btn->GetSprites())
	//			spr.lock()->SetRenderColor(released);
	//	});
	//}

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