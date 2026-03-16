#include "Game/GameObjects/CustomObjects/Character.h"

void Character::TakeDamage(float damage)
{
	if (m_isDead) return;
	
	// Take damage and cap min health to 0.0f
	m_health = std::fmax(0.0f, m_health - damage);

	// Reset time since last hit
	if (!m_hasBeenHit) m_hasBeenHit = true;
	m_timeSinceLastHit = 0.0f;

	// Call virtual
	OnTakeDamage(damage);
}

void Character::HealHealth(float health)
{
	if (m_isDead) return;
	
	// Take damage and cap min health to 0.0f
	m_health = std::fmin(m_health + health, m_maxHealth);
}

void Character::LerpWeaponOffset(float deltaTime)
{
	m_weaponOffset = glm::mix(m_weaponOffset, m_toBeWeaponOffset, deltaTime * m_speedADS);
}

void Character::OnTick(float deltaTime)
{
	// Lerp if weapon far from to-be offset
	if (glm::distance(m_weaponOffset, m_toBeWeaponOffset) > 0.01f) {
		LerpWeaponOffset(deltaTime);
	}

	// Manage hit awareness
	if (m_hasBeenHit) m_timeSinceLastHit += deltaTime;
	if (m_timeSinceLastHit > m_hitAwarenessTime) {
		m_hasBeenHit = false;
		m_timeSinceLastHit = 0.0f;
	}
}
