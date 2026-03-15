#pragma once
#include "EngineTypes.h"
#include "Game/EGameEngine.h"

class Floor;

class EObject : public std::enable_shared_from_this<EObject> {
public:
	EObject();
	virtual ~EObject();

	// Lambda bindings
	std::function<void(float)> OnTicked;
	std::function<void(float)> OnPostTicked;

	// Extend lambda bindings
	// Extends a binding
	template<typename TClass, typename... Args, typename Func>
	void ExtendBinding(std::function<void(Args...)> TClass::* target, Func func) {
		TClass* self = static_cast<TClass*>(this);
		auto existing = self->*target;
		self->*target = [existing, func](Args... args) {
			if (existing) existing(args...);
			func(args...);
			};
	}

	// Extends a binding with weak safety on 'this' only (no external object)
	template<typename TClass, typename Func, typename... Args>
	void BindSelfEvent(std::function<void(Args...)> TClass::* target, Func action) {
		static_assert(
			std::is_invocable_v<Func, TShared<TClass>, Args...>,
			"BindSelfEvent: action must take (TShared<TClass>, ...Args)"
			);
		auto self = GetWeakRef<TClass>();
		ExtendBinding(target, [self, action](Args... args) {
			if (const auto& s = self.lock())
				if constexpr (std::is_invocable_v<Func, TShared<TClass>, Args...>)
					action(s, args...);
			});
	}

	// Extends a binding with weak safety on 'this' and external object
	template<typename TClass, typename TPassed, typename Func, typename... Args>
	void BindObjectEvent(std::function<void(Args...)> TClass::* target, TWeak<TPassed> weak, Func action) {
		static_assert(
			std::is_invocable_v<Func, TShared<TClass>, TShared<TPassed>, Args...>,
			"BindObjectEvent: action must take (TShared<TClass>, TShared<TPassed>, ...Args)"
			);
		auto self = GetWeakRef<TClass>();
		ExtendBinding(target, [self, weak, action](Args... args) {
			if (const auto& s = self.lock()) {
				if (const auto& obj = weak.lock())
					if constexpr (std::is_invocable_v<Func, TShared<TClass>, TShared<TPassed>, Args...>)
						action(s, obj, args...);
			}
			});
	}

	#define BIND_EVENT_RAW(obj, event, action) \
			obj->ExtendBinding(&std::remove_reference_t<decltype(*obj)>::event, action)
	#define BIND_EVENT_SELF(obj, event, action) \
			obj->BindSelfEvent(&std::remove_reference_t<decltype(*obj)>::event, action)
	#define BIND_EVENT_EXT(obj, event, ...) \
			obj->BindObjectEvent(&std::remove_reference_t<decltype(*obj)>::event, __VA_ARGS__)

	// Run when the object spawns in
	void Start();

	// Run after the start for adding inputs
	void RegisterInputs(const TShared<EInput>& m_input);

	template<typename... Args, typename Func>
	void SetInputBinding(const TShared<EInput>& m_input, TShared<EEvents<Args...>> EInput::* event, Func func) {
		const auto& eventRef = ((*m_input).*event);
		EUi8 id = eventRef->Bind(func);
		m_inputUnbinds.push_back([eventRef, id]() { eventRef->Unbind(id); });
	}

	#define BIND_INPUT(input, event, action) \
			this->SetInputBinding(input, &EInput::event, action)

	// Run every frame, passes in deltaTime
	void Tick(float deltaTime);

	// Run after each tick every frame, passes in deltaTime
	void PostTick(float deltaTime);

	// Mark the object for destroy
	void Destroy();

	// Test if the object is marked for destroy
	bool IsPendingDestroy() const { return m_pendingDestroy; }

	// Get reference
	template<typename T>
	TWeak<T> GetWeakRef() {
		return std::static_pointer_cast<T>(shared_from_this());
	}

	template<typename T>
	TShared<T> GetSharedRef() {
		return std::static_pointer_cast<T>(shared_from_this());
	}

	// Set the lifetime of the object to be destroyed after seconds
	void SetLifeTime(float lifeTime) { 
		m_lifeTime = lifeTime; 
		m_lifeTimeTimer = m_lifeTime;
	}

	// Get the lifetime of the object to be destroyed after seconds
	glm::vec2 GetLifeTime() { return { m_lifeTime, m_lifeTimeTimer }; }

	// Ratio of time remaining before being destroyed after seconds
	float GetLifeTimeRatio() { return m_lifeTimeTimer / m_lifeTime; }

	// Render
	const bool ToggleRender() { m_doRender = !m_doRender; return m_doRender; }

	void SetDoRender(const bool newDoRender) { m_doRender = newDoRender; }

	const bool GetDoRender() { return m_doRender; }

protected:
	// Run when the object spawns in
	virtual void OnStart() {}

	// Run when the object spawns in
	virtual void OnRegisterInputs(const TShared<EInput>& m_input) {}

	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) {}

	// Run after each tick every frame, passes in deltaTime
	virtual void OnPostTick(float deltaTime) {}

	// Run when object is marked for destroy
	virtual void OnDestroy() {}

private:
	// If marked for destroy
	bool m_pendingDestroy;
	
	// If set, destroy object after time
	float m_lifeTime;

	// Time before destroy
	float m_lifeTimeTimer;

	// Whether object is rendered
	bool m_doRender;

	// Store unbinds for input bindings
	TArray<std::function<void()>> m_inputUnbinds;
};