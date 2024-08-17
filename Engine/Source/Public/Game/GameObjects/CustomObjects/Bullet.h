#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Bullet : public EWorldObject {
public:
	Bullet(glm::vec3& rotation, float damage, float moveSpeed);

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
};