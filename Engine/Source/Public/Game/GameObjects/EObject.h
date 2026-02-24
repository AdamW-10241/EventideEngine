#pragma once
#include "EngineTypes.h"
#include "Game/EGameEngine.h"

class Floor;

class EObject : public std::enable_shared_from_this<EObject> {
public:
	EObject();
	virtual ~EObject();

	// Run when the object spawns in
	void Start();

	// Run after the start for adding inputs
	void RegisterInputs(const TShared<EInput>& m_input);

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

private:
	// If marked for destroy
	bool m_pendingDestroy;
	
	// If set, destroy object after time
	float m_lifeTime;

	// Time before destroy
	float m_lifeTimeTimer;

private:
	// Whether object is rendered
	bool m_doRender;
};