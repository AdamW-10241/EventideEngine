#pragma once
#include "EngineTypes.h"
#include "Game/EGameEngine.h"

class EObject : public std::enable_shared_from_this<EObject> {
public:
	EObject();
	virtual ~EObject();

	// Run when the object spawns in
	void Start();

	// Run every frame, passes in deltaTime
	void Tick(float deltaTime);

	// Run after each tick every frame, passes in deltaTime
	void PostTick(float deltaTime);

	// Mark the object for destroy
	void Destroy();

	// Test if the object is marked for destroy
	bool IsPendingDestroy() const { return m_pendingDestroy; }

	// Set the lifetime of the object to be destroyed after seconds
	void SetLifeTime(float lifeTime) { 
		m_lifeTime = lifeTime; 
		m_lifeTimeTimer = m_lifeTime;
	}

protected:
	// Run when the object spawns in
	virtual void OnStart() {}

	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) {}

	// Run after each tick every frame, passes in deltaTime
	virtual void OnPostTick(float deltaTime) {}

private:
	// If marked for destroy
	bool m_pendingDestroy;
	
	// If set, destroy object after time
	float m_lifeTime;

	// Time before destroy
	float m_lifeTimeTimer;
};