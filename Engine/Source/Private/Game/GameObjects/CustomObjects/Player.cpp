#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"

#define Super EWorldObject

void Player::SetDefaultCamPosition(glm::vec3 position)
{
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		camRef->transform.position = position;
	}
}

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f) }, true).lock()) {
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

void Player::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{	
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		// Reverse last camera movement
		camRef->transform.position -= camRef->lastMovement * 2.0f;
		// Reset last movement
		camRef->lastMovement = glm::vec3(0.0f);
		// Update transform
		GetTransform() = camRef->transform;
	}
}
