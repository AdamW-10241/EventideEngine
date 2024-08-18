#pragma once
#include "Game/GameObjects/CustomObjects/Character.h"

class Player;

class Enemy : public Character {
public:
	Enemy(TWeak<Player> playerRef);

	// Spawn another enemy
	static void SpawnEnemy(TWeak<Player> playerRef);

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;

	virtual void OnPostTick(float deltaTime) override;

	virtual void OnTakeDamage(float damage) override;

private:
	TWeak<Player> m_playerRef;

	// Store the weapon offset
	glm::vec3 m_weaponOffset;

	// Store the coin spawn offset
	glm::vec3 m_coinSpawnOffset;
};