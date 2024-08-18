#include "Game/GameObjects/CustomObjects/Enemy.h"
#include "Graphics/EMesh.h"
#include "Game/GameObjects/CustomObjects/Floor.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"

#define Super Character

Enemy::Enemy()
{
	m_health = 3.0f;
}

void Enemy::OnStart()
{
	Super::OnStart();
	
	// Place randomly on floor mesh
	if (const auto& floor = EGameEngine::GetGameEngine()->FindObjectOfType<Floor>().lock()) {
		PlaceOnFloorRandomly(floor, 25.0f);
	}

	// Adjust upward
	GetTransform().position.y += 15.0f;

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(10.0f) }, true).lock()) {
		colRef->type = EECollisionType::ENEMY;
	}
}

void Enemy::OnTick(float deltaTime)
{
	// Move weapon to enemy
	m_weapon->GetTransform().position = GetTransform().position;
}

void Enemy::OnPostTick(float deltaTime)
{
	Super::OnPostTick(deltaTime);
	
	// Keep attempting to fire
	m_weapon->TryFire(EECollisionType::BULLET_ENEMY, {0.0f, 0.0f, 0.0f});
}

void Enemy::OnTakeDamage(float damage)
{
	Super::OnTakeDamage(damage);
	
	if (m_health <= 0.0) {
		Destroy();
	}
}
