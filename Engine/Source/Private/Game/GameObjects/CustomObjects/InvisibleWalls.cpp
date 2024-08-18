#include "Game/GameObjects/CustomObjects/InvisibleWalls.h"

#define Super EWorldObject

void InvisibleWalls::OnStart()
{
	Super::OnStart();

	// Add collisions for each direction
	AddCollision({ glm::vec3(0.0f, 0.0f, 300.0f), glm::vec3(300.0f, 50.0f, 5.0f) }, false).lock()->type = EECollisionType::WALL;
	AddCollision({ glm::vec3(0.0f, 0.0f, -300.0f), glm::vec3(300.0f, 50.0f, 5.0f) }, false).lock()->type = EECollisionType::WALL;
	AddCollision({ glm::vec3(300.0f, 0.0f, 0.0f), glm::vec3(5.0f, 50.0f, 300.0f) }, false).lock()->type = EECollisionType::WALL;
	AddCollision({ glm::vec3(-300.0f, 0.0f, 0.0f), glm::vec3(5.0f, 50.0f, 300.0f) }, false).lock()->type = EECollisionType::WALL;
}
