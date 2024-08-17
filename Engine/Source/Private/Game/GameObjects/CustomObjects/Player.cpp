#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"

#define Super EWorldObject

void Player::SetDefaultCamPosition(glm::vec3 position)
{
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		camRef->transform.position = position;
	}
}

void Player::OnStart()
{
	Super::OnStart();
	
	// If camera exists,
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		// Move to camera
		GetTransform().position = camRef->transform.position;
		// Set old position (for next loop)
		m_oldPosition = GetTransform().position;
	}

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f) }, true).lock()) {
		colRef->type = EECollisionType::PLAYER;
	}
}

void Player::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);

	// Reset collide flag
	m_collided = false;

	// Move to camera
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		GetTransform().position = camRef->transform.position;

		// If weapon exists,
		if (m_weapon) {
			// Move and offset
			//m_weapon->GetTransform().position = (camRef->transform.position + m_weaponOffset) *

			// Rotate
			m_weapon->GetTransform().rotation.y = camRef->transform.rotation.y + 180;
		}
	}
}

void Player::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{	
	Super::OnOverlap(other, col, otherCol);
	
	// Set collide flag
	m_collided = true;
}

void Player::OnPostTick(float deltaTime)
{
	Super::OnPostTick(deltaTime);

	// Move to camera, or reverse frame camera movement, based on collisions
	if (const auto& camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		if (m_collided) {
			// Reset camera position to before collision
			camRef->transform.position = m_oldPosition;
			// Reset player position to before collision
			GetTransform().position = m_oldPosition;
		}
	}

	// Store old position (old for next loop)
	m_oldPosition = GetTransform().position;
}
