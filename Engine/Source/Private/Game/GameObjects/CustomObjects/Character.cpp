#include "Game/GameObjects/CustomObjects/Character.h"

void Character::TakeDamage(float damage)
{
	// Take damage and cap min health to 0.0f
	m_health = std::fmax(0.0f, m_health - damage);

	// Call virtual
	OnTakeDamage(damage);
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
}
