#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"

#define Super EWorldObject

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f) }, false).lock()) {
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