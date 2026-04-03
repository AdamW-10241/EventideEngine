#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Game/GameObjects/CustomObjects/Bullet.h"
#include "Graphics/EGraphicsEngine.h"
#include "Game/GameObjects/CustomObjects/Player.h"

#define Super EWorldObject

Weapon::Weapon(TWeak<Character> owner, bool addModel, float damage, float moveSpeed, float attackCooldownTime, bool unlimitedAmmo)
{
	m_owner = owner;
	m_addModel = addModel;

	m_barrelLength = 8.0f;
	m_bulletDamage = damage;
	m_bulletMoveSpeed = moveSpeed;
	m_attackCooldownTime = attackCooldownTime;
	m_unlimitedAmmo = unlimitedAmmo;
	
	m_attackTimer = m_attackCooldownTime;

	m_magazineSize = 16;
	m_magazineAmmo = m_magazineSize;
	m_reserveAmmo = 64;
}

bool Weapon::TryFire(EECollisionType bulletCollisionType, glm::vec3 shootDirection)
{
	bool fired = false;

	// If timer expired
	if (m_attackTimer <= 0.0f) {
		fired = Fire(bulletCollisionType, shootDirection);
		m_attackTimer = m_attackCooldownTime;
	}

	return fired;
}

bool Weapon::Fire(EECollisionType bulletCollisionType, glm::vec3 shootDirection)
{
	glm::vec3 spawnPos = GetTransform().position + shootDirection * m_barrelLength;
	float pitch = glm::degrees(asin(shootDirection.y));
	float yaw = glm::degrees(atan2(shootDirection.x, shootDirection.z));
	glm::vec3 spawnRot = glm::vec3(-pitch, yaw, 0.0f);
	float lifetime = 3.0f;

	auto bullet = EGameEngine::GetGameEngine()->CreateObject<Bullet>(bulletCollisionType, glm::vec3(0, 0, 1), m_bulletDamage, m_bulletMoveSpeed, lifetime, spawnPos, spawnRot);
	bool fired = !bullet.expired();

	//auto player = EGameEngine::GetGameEngine()->GetPlayer().lock();
	//if (!player) {
	//	EDebug::Log(LT_WARNING, "Player does not exist to scale fired bullet audio.");
	//	return fired;
	//}

	//// Distance scale audio
	//if (auto soundManager = EGameEngine::GetGameEngine()->GetSoundManager().lock()) {
	//	float distance = glm::distance(spawnPos, player->GetTransform().position);
	//	int volume = 50 - int(distance / 5);
	//	soundManager->PlaySound(EE_SOUND_SHOOT, volume);
	//}

	return fired;
}

void Weapon::Reload()
{
	// Would reload and use magazine if UI was developed to visualise it
}

void Weapon::OnStart()
{
	Super::OnStart();
	
	// Return if not adding model
	if (!m_addModel) return;

	// Scale gun
	GetTransform().scale = glm::vec3(0.01f);

	// Add model
	EString modelPath = "Models/Gun/sg553_flipped.fbx";
	TArray<ESMaterialSlot> materials = {
		{ {0}, ESMaterialDesc{
			{"Models/Gun/textures/sg553_BaseColor.tga.png",
			 "Models/Gun/textures/sg553_Normal.tga.png",
			 "Models/Gun/textures/sg553_Metalness.tga.png"}
		} } 
	};
	auto model = LoadModel(modelPath, materials);
}

void Weapon::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);

	// Decrement timer by delta time
	if (m_attackTimer > 0.0f)
		m_attackTimer -= deltaTime;
}
