#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Weapon;

class Character : public EWorldObject {
public:
	Character() { m_health = 1.0f;}

	// Add a weapon to the player
	void AddWeapon(TShared<Weapon>& weapon) { m_weapon = weapon; }

	// Get the characters health
	float GetHealth() const { return m_health; }
	
	// Take damage
	void TakeDamage(float damage);

protected:
	// Virtual take damage function
	virtual void OnTakeDamage(float damage) {}


protected:
	// Store the characters health
	float m_health;

	// Store the characters weapon
	TShared<Weapon> m_weapon;
};