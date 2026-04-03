#include "Game/GameObjects/EObject.h"

EObject::EObject()
{
	m_pendingDestroy = false;
	m_lifeTime = 0.0f;
	m_lifeTimeTimer = 0.0f;
	m_doRender = true;
}

void EObject::Start()
{
	OnStart();
}

void EObject::RegisterInputs(const TShared<EInput>& m_input)
{
	OnRegisterInputs(m_input);
}

void EObject::Tick(float deltaTime)
{
	OnTick(deltaTime);

	// Run lambdas
	if (OnTicked) OnTicked(deltaTime);

	if (m_lifeTime > 0.0f) {
		// Count down the lifetime timer
		m_lifeTimeTimer -= deltaTime;
	}

	// When the timer hits less than 0, destroy the object
	if (m_lifeTimeTimer < 0.0f) {
		Destroy();
	}
}

void EObject::PostTick(float deltaTime)
{
	OnPostTick(deltaTime);

	// Run lambdas
	if (OnPostTicked) OnPostTicked(deltaTime);
}

void EObject::Destroy()
{
	m_pendingDestroy = true;

	// Call all input unbinds
	for (std::function<void()> unbind : m_inputUnbinds) {
		unbind();
	}

	OnDestroy();
	EGameEngine::GetGameEngine()->DestroyObject(shared_from_this());
}