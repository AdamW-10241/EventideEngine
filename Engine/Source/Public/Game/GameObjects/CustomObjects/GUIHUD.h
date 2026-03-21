#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIHUD : public EScreenObject {
public:
	GUIHUD() {}

	// Stores a screen object
	template<typename T>
		requires std::derived_from<T, EScreenObject>
	TWeak<T> AddScreenObject(TWeak<T> screenObject);

	// Pass in arguments to create screen object and add to array
	template<typename T, typename ...Args>
		requires std::derived_from<T, EScreenObject>
	TWeak<T> AddScreenObject(Args&&... args);

    // Get screen objects
    TArray<TWeak<EScreenObject>> GetScreenObjects() { return m_screenObjects; }

    TWeak<EScreenObject> GetScreenObject(const int index) {
        if ((int)m_screenObjects.size() > index) {
            return m_screenObjects.at(index);
        }
        return {};
    }

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

protected:
	TArray<TWeak<EScreenObject>> m_screenObjects;

};

template<typename T>
	requires std::derived_from<T, EScreenObject>
inline TWeak<T> GUIHUD::AddScreenObject(TWeak<T> screenObject)
{
	// Add to array and return it
	if (auto ref = screenObject.lock()) {
		m_screenObjects.push_back(ref);
		return ref;
	}
	return {};
}

template<typename T, typename ...Args>
	requires std::derived_from<T, EScreenObject>
inline TWeak<T> GUIHUD::AddScreenObject(Args&&... args)
{
	// Create and add screen object
	auto screenObject = EGameEngine::GetGameEngine()->CreateObject<T>(std::forward<Args>(args)...);
	return AddScreenObject(screenObject);
}
