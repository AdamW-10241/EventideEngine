#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Bullet : public EWorldObject {
public:
	Bullet(EECollisionType collisionType, glm::vec3 moveVector, float damage, float moveSpeed, float lifetime, glm::vec3 spawnPos, glm::vec3 spawnRot);

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;

private:
	// Store the bullet damage
	float m_damage;

	// Store the bullets move speed;
	float m_moveSpeed;

	// Store the movement vector for the bullet
	glm::vec3 m_moveVector;

	// Store the collision type
	EECollisionType m_collisionType;
};