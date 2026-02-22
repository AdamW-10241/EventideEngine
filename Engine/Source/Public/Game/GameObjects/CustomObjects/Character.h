#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Weapon;

class Character : public EWorldObject {
public:
	Character() {
		m_health = 1.0f; 
		m_isWeaponADS = false;
		m_speedADS = 20.0f;
		m_weaponOffset = glm::vec3(0.0f);
		m_weaponBaseOffset = glm::vec3(0.0f);
		m_weaponADSOffset = glm::vec3(0.0f);
		m_toBeWeaponOffset = glm::vec3(0.0f);
	}

	// Add a weapon to the character
	void AddWeapon(TShared<Weapon>& weapon) { m_weapon = weapon; }

	// Get the characters health
	float GetHealth() const { return m_health; }
	
	// Take damage
	void TakeDamage(float damage);

	// Set to-be weapon offset
	void SetToBeWeaponOffset(glm::vec3 toBeWeaponOffset) { m_toBeWeaponOffset = toBeWeaponOffset; }

	// Lerp weapon offset
	void LerpWeaponOffset(float deltaTime);

protected:
	// Virtual take damage function
	virtual void OnTakeDamage(float damage) {}

	virtual void OnTick(float deltaTime) override;

protected:
	// Store the characters health
	float m_health;

	// Store the characters weapon
	TShared<Weapon> m_weapon;

	// Store if weapon is ADS
	bool m_isWeaponADS;

	// Store the weapon ADS speed
	float m_speedADS;

	// Store the weapon offsets
	glm::vec3 m_weaponOffset;
	glm::vec3 m_weaponBaseOffset;
	glm::vec3 m_weaponADSOffset;
	glm::vec3 m_toBeWeaponOffset;
};