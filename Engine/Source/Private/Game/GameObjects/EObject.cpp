#include "Game/GameObjects/EObject.h"

EObject::EObject()
{
	m_pendingDestroy = false;
	m_lifeTime = 0.0f;
	m_lifeTimeTimer = 0.0f;
	
	EDebug::Log("EObject created");
}

EObject::~EObject()
{
	EDebug::Log("EObject destroyed");
}

void EObject::Start()
{
	OnStart();
}

void EObject::Tick(float deltaTime)
{
	OnTick(deltaTime);

	if (m_lifeTime > 0.0f) {
		// Count down the lifetime timer
		m_lifeTimeTimer -= deltaTime;
	}

	// When the timer hits 0, destroy the object
	if (m_lifeTimeTimer <= 0.0f) {
		Destroy();
	}
}

void EObject::PostTick(float deltaTime)
{
	OnPostTick(deltaTime);
}

void EObject::Destroy()
{
	m_pendingDestroy = true;

	EGameEngine::GetGameEngine()->DestroyObject(shared_from_this());
}
