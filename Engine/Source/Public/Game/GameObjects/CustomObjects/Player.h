#pragma once
#include "Game/GameObjects/CustomObjects/Character.h"

struct ESSpotLight;
class Weapon;
class EScreenObject;

class Player : public Character {
public:
	Player();

	// Set the cameras default position
	void SetDefaultCamPosition(glm::vec3 position);

	// Add a crosshair to the character
	void AddCrosshair(TShared<EScreenObject> crosshair) { m_crosshair = crosshair; }

protected:
	virtual void OnStart() override;

	virtual void OnRegisterInputs(const TShared<EInput>& m_input) override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;

	virtual void OnPostTick(float deltaTime) override;

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

	// Store the crosshair attached to the player
	TShared<EScreenObject> m_crosshair;
};