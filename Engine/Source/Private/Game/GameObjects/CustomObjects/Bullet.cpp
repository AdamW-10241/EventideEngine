#include "Game/GameObjects/CustomObjects/Bullet.h"
#include "Math/ESCollision.h"
#include "Game/GameObjects/CustomObjects/Enemy.h"

#define Super EWorldObject

Bullet::Bullet(EECollisionType collisionType, glm::vec3 moveVector, float damage, float moveSpeed)
{
	m_collisionType = collisionType;
	m_moveVector = moveVector;
	m_damage = damage;
	m_moveSpeed = moveSpeed;
}

void Bullet::OnStart()
{
	Super::OnStart();

	// Scale floor
	GetTransform().scale = glm::vec3(0.01f);

	// Add model
	auto model = ImportModel("Models/Bullet/CubeBullet.fbx");

	// Gun base
	auto bulletBase = TMakeShared<ETexture>();
	bulletBase->LoadTexture("Bullet Base", "Textures/T_Brass.png");

	// Creating materials
	auto bulletMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	bulletMat->m_baseColourMap = bulletBase;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, bulletMat);

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(4.0f, 4.0f, 4.0f) }, true).lock()) {
		colRef->type == m_collisionType;
	}
}

void Bullet::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);
	
	// Move forward locally
	TranslateLocal(deltaTime, m_moveVector, glm::vec3(m_moveSpeed));
}

void Bullet::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{
	Super::OnOverlap(other, col, otherCol);

	// Check overlap types
	if (m_collisionType == EECollisionType::BULLET_PLAYER && otherCol->type == EECollisionType::ENEMY ||
		m_collisionType == EECollisionType::BULLET_ENEMY && otherCol->type == EECollisionType::PLAYER) {
		// Hit player or enemy with counterpart bullet
		if (const auto& character = std::dynamic_pointer_cast<Character>(other)) {
			// Deal damage
			character->TakeDamage(m_damage);
			// Destroy after overlap
			Destroy();
		}
	}

	// On overlap,
	switch (otherCol->type)	{
	case EECollisionType::ALL:
	case EECollisionType::WALL:
		Destroy();
		break;
	}
}
