#include "Game/GameObjects/CustomObjects/Bullet.h"

#define Super EWorldObject

Bullet::Bullet(glm::vec3& rotation, float damage, float moveSpeed)
{
	GetTransform().rotation = rotation;
	m_damage = damage;
	m_moveSpeed = moveSpeed;
}

void Bullet::OnStart()
{
	Super::OnStart();
}

void Bullet::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);
	
	// Move forward locally
	TranslateLocal(deltaTime, GetTransform().rotation, glm::vec3(m_moveSpeed));
}

void Bullet::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{
	Super::OnOverlap(other, col, otherCol);

	Destroy();
}
