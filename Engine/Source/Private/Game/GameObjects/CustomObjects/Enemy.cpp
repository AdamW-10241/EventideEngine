#include "Game/GameObjects/CustomObjects/Enemy.h"
#include "Graphics/EMesh.h"
#include "Game/GameObjects/CustomObjects/Floor.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Game/GameObjects/CustomObjects/Player.h"
#include "Game/GameObjects/CustomObjects/Coin.h"

#define Super Character

Enemy::Enemy(TWeak<Player> playerRef)
{
	m_playerRef = playerRef;
	
	m_health = 3.0f;

	m_weaponOffset = { 0.0f, 10.0f, 0.0f };
	m_coinSpawnOffset = { 0.0f, 5.0f, 0.0f };
}

void Enemy::OnStart()
{
	Super::OnStart();
	
	// Place randomly on floor mesh
	if (const auto& floor = EGameEngine::GetGameEngine()->FindObjectOfType<Floor>().lock()) {
		PlaceOnFloorRandomly(floor, 25.0f);
	}

	// Adjust upward
	GetTransform().position.y += 5.0f;

	// Scale enemy
	GetTransform().scale = glm::vec3(15.0f);

	// Add model
	auto model = ImportModel("Models/Enemy/scene.gltf");

	// Enemy base
	auto enemyBase = TMakeShared<ETexture>();
	enemyBase->LoadTexture("Enemy Base", "Models/Enemy/textures/m_828990a1-ca37-b194-653d-836ec5c4f93b_baseColor.png");

	// Creating materials
	auto enemyMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	enemyMat->m_baseColourMap = enemyBase;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, enemyMat);

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f, 20.0f, 5.0f)}, false).lock()) {
		colRef->type = EECollisionType::ENEMY;
	}
}

void Enemy::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);
	
	// Move weapon to enemy
	m_weapon->GetTransform().position = GetTransform().position + m_weaponOffset;
}

void Enemy::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{
	if (otherCol->type == EECollisionType::WALL) {
		// Spawn new enemy
		Enemy::SpawnEnemy(m_playerRef);

		// Destroy self
		Destroy();
	}
}

void Enemy::OnPostTick(float deltaTime)
{
	Super::OnPostTick(deltaTime);
	
	// Look at player
	if (const auto& player = m_playerRef.lock()) {
		// Calculate target angle with atan2 from enemy and player positions
		float targetAngle = glm::degrees(atan2(player->GetTransform().position.x - GetTransform().position.x,
			player->GetTransform().position.z - GetTransform().position.z));

		// Get the closest difference with mod
		float angleDifference = fmod(targetAngle - GetTransform().rotation.y + 180.0f, 360.0f) - 180.0f;

		// Add the angle difference and look at the player
		GetTransform().rotation.y += angleDifference + 180.0f;
	}

	// Keep attempting to fire
	m_weapon->TryFire(EECollisionType::BULLET_ENEMY, {0.0f, GetTransform().rotation.y + 180.0f, 0.0f });
}

void Enemy::OnTakeDamage(float damage)
{
	Super::OnTakeDamage(damage);
	
	// Check if dead
	if (m_health <= 0.0) {
		// Spawn coin
		if (const auto& coinRef = EGameEngine::GetGameEngine()->CreateObject<Coin>().lock()) {
			coinRef->GetTransform().position = GetTransform().position + m_coinSpawnOffset;
		}

		// Spawn new enemy
		Enemy::SpawnEnemy(m_playerRef);

		// Destroy self
		Destroy();
	}
}

void Enemy::SpawnEnemy(TWeak<Player> playerRef)
{
	// Spawn a new enemy
	if (const auto& newEnemy = EGameEngine::GetGameEngine()->CreateObject<Enemy>(playerRef).lock()) {
		// Random fire rate
		float fireRate = EGameEngine::GetGameEngine()->GetRandomFloatRange(0.8f, 1.2f);
		// Add weapon
		if (TShared<Weapon> weapon = EGameEngine::GetGameEngine()->CreateObject<Weapon>(false, 1.0f, 100.0f, fireRate, true).lock()) {
			newEnemy->AddWeapon(weapon);
		}
	}
}
