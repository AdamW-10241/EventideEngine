#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Weapon;

class Player : public EWorldObject {
public:
	Player() { m_collided = false; m_oldPosition = glm::vec3(0.0f); m_weaponOffset = glm::vec3(-5.0f, 0.0f, 0.0f); }

	// Set the cameras default position
	void SetDefaultCamPosition(glm::vec3 position);

	// Add a weapon to the player
	void AddWeapon(TShared<Weapon>& weapon) { m_weapon = weapon; }

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;

	virtual void OnPostTick(float deltaTime) override;

private:
	// Bool for if collided this frame
	bool m_collided;

	// Store the old position to restore the camera and object
	glm::vec3 m_oldPosition;

	// Store the weapon
	TShared<Weapon> m_weapon;

	// Store the weapon offset from the player
	glm::vec3 m_weaponOffset;

};