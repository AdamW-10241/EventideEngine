#pragma once
#include "Game/GameObjects/CustomObjects/Character.h"

struct ESSpotLight;
class Weapon;
class EScreenObject;

class Player : public Character {
public:
	Player(glm::vec3 spawnLocation);

protected:
	virtual void OnStart() override;

	virtual void OnRegisterInputs(const TShared<EInput>& m_input) override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;

	virtual void OnPostTick(float deltaTime) override;

	virtual void OnTakeDamage(float damage) override;

	virtual void OnDeath() override;

private:
	void SetSpawnLocation(glm::vec3 position);

private:
	// Bool for if collided this frame
	bool m_collided;

	// Store the old position to restore the camera and object
	glm::vec3 m_oldPosition;

	// Store if the player is currently pressing left mouse
	bool m_leftMouseHeld;

	// Store if the player is currently pressing right mouse
	bool m_rightMouseHeld;

	// Store the spotlight attached to the player
	TShared<ESSpotLight> m_light;

	// Store the screenObjects attached to the player
	TWeak<EScreenObject> m_crosshair;
	TWeak<EScreenObject> m_hitOverlay;
};