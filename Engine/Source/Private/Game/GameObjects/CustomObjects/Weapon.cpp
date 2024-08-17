#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Game/GameObjects/CustomObjects/Bullet.h"

Weapon::Weapon(float damage, float moveSpeed, float attackCooldownTime, bool unlimitedAmmo)
{
	m_bulletDamage = damage;
	m_bulletMoveSpeed = moveSpeed;
	m_attackCooldownTime = attackCooldownTime;
	m_unlimitedAmmo = unlimitedAmmo;
	
	m_attackTimer = m_attackCooldownTime;

	m_magazineSize = 16;
	m_magazineAmmo = m_magazineSize;
	m_reserveAmmo = 64;
}

void Weapon::Fire()
{
	// Summon bullet
	if (const auto& bullet = EGameEngine::GetGameEngine()->CreateObject<Bullet>(GetTransform().rotation, m_bulletDamage, m_bulletMoveSpeed).lock()) {
		// Set position to gun
		bullet->GetTransform().position = GetTransform().position;
	}
}

void Weapon::Reload()
{

}

void Weapon::OnStart()
{
	// Scale floor
	GetTransform().scale = glm::vec3(0.01f);

	// Add model
	auto model = ImportModel("Models/Gun/sg553.fbx");

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
