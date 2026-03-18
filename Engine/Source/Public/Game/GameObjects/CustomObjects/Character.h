#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Weapon;

class Character : public EWorldObject {
public:
	Character() {
		m_maxHealth = 1.0f;
		ResetHealth();
		m_isDead = false;
		m_hasBeenHit = false;
		m_healRate = -1.0f;
		m_timeSinceLastHit = 0.0f;
		m_hitAwarenessTime = 3.0f;
		m_isWeaponADS = false;
		m_speedADS = 30.0f;
		m_weaponOffset = glm::vec3(0.0f);
		m_weaponBaseOffset = glm::vec3(0.0f);
		m_weaponADSOffset = glm::vec3(0.0f);
		m_toBeWeaponOffset = glm::vec3(0.0f);
	}

	// Add a weapon to the character
	void AddWeapon(TShared<Weapon>& weapon) { m_weapon = weapon; }

	// Set the characters health
	void SetHealth(float health) { m_health = health; }
	void ResetHealth() { m_health = m_maxHealth; }

	// Get the characters health
	float GetHealth() const { return m_health; }

	// Ratio of health remaining
	float GetHealthRatio() const { return m_health / m_maxHealth; }

	// Get if has been hit (recently)
	bool GetHasBeenHit() const { return m_hasBeenHit; }
	
	// Take damage
	void TakeDamage(float damage);

	// Heal health
	void HealHealth(float health);

	// Set to-be weapon offset
	void SetToBeWeaponOffset(glm::vec3 toBeWeaponOffset) { m_toBeWeaponOffset = toBeWeaponOffset; }

	// Lerp weapon offset
	void LerpWeaponOffset(float deltaTime);

protected:
	virtual void OnTick(float deltaTime) override;

	virtual void OnTakeDamage(float damage) {}

	virtual void OnDeath() {};

protected:
	// Store the characters health
	float m_health;
	float m_maxHealth;
	float m_isDead;

	// Store heal rate
	float m_healRate;

	// Store time since last hit
	bool m_hasBeenHit;
	float m_timeSinceLastHit;

	// Store time until character will forget they have been hit
	float m_hitAwarenessTime;

	// Store the characters weapon
	TWeak<Weapon> m_weapon;

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