#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Game/GameObjects/CustomObjects/Bullet.h"

#define Super EWorldObject

Weapon::Weapon(bool addModel, float damage, float moveSpeed, float attackCooldownTime, bool unlimitedAmmo)
{
	m_addModel = addModel;

	m_bulletDamage = damage;
	m_bulletMoveSpeed = moveSpeed;
	m_attackCooldownTime = attackCooldownTime;
	m_unlimitedAmmo = unlimitedAmmo;
	
	m_attackTimer = m_attackCooldownTime;

	m_magazineSize = 16;
	m_magazineAmmo = m_magazineSize;
	m_reserveAmmo = 64;
}

void Weapon::TryFire(EECollisionType bulletCollisionType, glm::vec3 bulletRotation)
{
	// If timer expired,
	if (m_attackTimer <= 0.0f) {
		// Fire gun
		Fire(bulletCollisionType, bulletRotation);
		// Reset timer
		m_attackTimer = m_attackCooldownTime;
	}
}

void Weapon::Fire(EECollisionType bulletCollisionType, glm::vec3 bulletRotation)
{
	// Set movement vector (forward)
	glm::vec3 movementVector = { 0.0f, 0.0f, 1.0f };

	// Spawn bullet
	if (const auto& bullet = EGameEngine::GetGameEngine()->CreateObject<Bullet>(bulletCollisionType, movementVector, m_bulletDamage, m_bulletMoveSpeed).lock()) {
		// Set position, rotation and lifetime
		bullet->GetTransform().position = GetTransform().position;
		bullet->GetTransform().rotation = bulletRotation;
		bullet->SetLifeTime(3.0f);
	}
}

void Weapon::Reload()
{

}

void Weapon::OnStart()
{
	Super::OnStart();
	
	// Return if not adding model
	if (!m_addModel) return;

	// Scale gun
	GetTransform().scale = glm::vec3(0.01f);

	// Add model
	auto model = ImportModel("Models/Gun/sg553_flipped.fbx");

	// Gun base
	auto gunBase = TMakeShared<ETexture>();
	gunBase->LoadTexture("Gun Base", "Models/Gun/textures/sg553_BaseColor.tga.png");
	// Gun normal
	auto gunNorm = TMakeShared<ETexture>();
	gunNorm->LoadTexture("Gun Normal", "Models/Gun/textures/sg553_Normal.tga.png");
	// Gun specular
	auto gunSpec = TMakeShared<ETexture>();
	gunSpec->LoadTexture("Gun Normal", "Models/Gun/textures/sg553_Metalness.tga.png");

	// Creating materials
	auto gunMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	gunMat->m_baseColourMap = gunBase;
	gunMat->m_normalMap = gunNorm;
	gunMat->m_specularMap = gunSpec;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, gunMat);
}

void Weapon::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);

	// Decrement timer by delta time
	if (m_attackTimer > 0.0f)
		m_attackTimer -= deltaTime;
}
