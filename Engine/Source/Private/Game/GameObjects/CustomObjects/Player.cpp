#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"

#define Super EWorldObject

Player::Player() {}

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(10.0f) }).lock()) {
		colRef->type = EECollisionType::PLAYER;
	}
}

void Player::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);
	
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		GetTransform() = camRef->transform;
	}
}
