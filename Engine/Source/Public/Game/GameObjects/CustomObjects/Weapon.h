#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Weapon : public EWorldObject {
public:
	Weapon(bool addModel = true, float damage = 1.0f, float moveSpeed = 0.1f, float attackCooldownTime = 1.0f, bool unlimitedAmmo = false);

	// Try to fire
	void TryFire(EECollisionType bulletCollisionType, glm::vec3 bulletRotation);

	// Reload the weapon and adjust ammo stocks
	void Reload();

	// Get the ammo in reserve
	EUi32 GetReserveAmmo() const { return m_reserveAmmo; }

	// Get the ammo in reserve
	EUi32 GetMagazineAmmo() const { return m_magazineAmmo; }

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

private:
	// Fire the gun to spawn and bullet
	void Fire(EECollisionType bulletCollisionType, glm::vec3 bulletRotation);

private:
	// Store whether the model should be added
	bool m_addModel;

	// Damage dealt by the fired bullet
	float m_bulletDamage;

	// Move speed of the spawned bullet
	float m_bulletMoveSpeed;

	// Time between attacks
	float m_attackCooldownTime;

	// Time until next attack
	float m_attackTimer;

	// Ammo in reserve
	EUi32 m_reserveAmmo;

	// Ammo in the magazine
	EUi32 m_magazineAmmo;

	// Size of the magazine
	EUi32 m_magazineSize;

	// Flag for if the weapon should have unlimited ammo
	bool m_unlimitedAmmo;
};