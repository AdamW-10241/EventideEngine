#include "Game/GameObjects/CustomObjects/Character.h"

void Character::TakeDamage(float damage)
{
	// Take damage and cap min health to 0.0f
	m_health = std::fmax(0.0f, m_health - damage);

	// Call virtual
	OnTakeDamage(damage);
}
