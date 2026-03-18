#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIHUD : public EScreenObject {
public:
	GUIHUD() {}

	// Stores a screen object
	TWeak<EScreenObject> AddScreenObject(TWeak<EScreenObject> screenObject);

	// Pass in arguments to create screen object and add to array
	template<typename... Args>
	TWeak<EScreenObject> AddScreenObject(Args&&... args);

    // Get screen objects
    TArray<TWeak<EScreenObject>> GetScreenObjects() { return m_screenObjects; }

    TWeak<EScreenObject> GetScreenObject(const int index) {
        if ((int)m_screenObjects.size() > index) {
            return m_screenObjects.at(index);
        }
        return {};
    }

	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) override;

protected:
	TArray<TWeak<EScreenObject>> m_screenObjects;

};

template<typename ...Args>
inline TWeak<EScreenObject> GUIHUD::AddScreenObject(Args&&... args)
{
	// Create and add screen object
	auto screenObject = EGameEngine::GetGameEngine()->CreateObject<EScreenObject>(std::forward<Args>(args)...);
	return AddScreenObject(screenObject);
}
