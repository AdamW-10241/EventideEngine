#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/euler_angles.hpp>

#define Super Character

Player::Player()
{
	m_health = 10.0f;
	
	m_collided = false;
	m_oldPosition = glm::vec3(0.0f);
	m_weaponOffset = glm::vec3(2.0f, -3.0f, 2.0f);

	m_leftMouseHeld = false;
}

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
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f) }, false).lock()) {
		colRef->type = EECollisionType::PLAYER;
	}
}

void Player::OnRegisterInputs(const TShared<EInput>& m_input)
{
	// Mouse pressed
	m_input->OnMousePressed->Bind([this](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			m_leftMouseHeld = true;
		}
	});

	// Mouse released
	m_input->OnMouseReleased->Bind([this](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			m_leftMouseHeld = false;
		}
	});
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
			// Move to offset
			// Get camera rotation in radians
			glm::vec3 cameraRotationRadians = glm::radians(camRef->transform.rotation);

			// Get camera rotation as a matrix to calculate local offset with rotation
			glm::mat4 cameraRotationMatrix = glm::eulerAngleXYZ(cameraRotationRadians.x, cameraRotationRadians.y, cameraRotationRadians.z);

			// Get rotated offset
			glm::vec3 rotatedWeaponOffset = glm::vec3(glm::inverse(cameraRotationMatrix) * glm::vec4(m_weaponOffset, 0.0f));

			// Set position and rotation
			m_weapon->GetTransform().position = camRef->transform.position + rotatedWeaponOffset;
			m_weapon->GetTransform().rotation = camRef->transform.rotation;

			// Fire weapon is holding left mouse
			if (m_leftMouseHeld)
				m_weapon->TryFire(EECollisionType::BULLET_PLAYER, camRef->transform.rotation);
		}
	}
}

void Player::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{	
	Super::OnOverlap(other, col, otherCol);
	
	// Delete wall if colliding with player on spawn
	if (otherCol->type == EECollisionType::WALL) {
		// If wall has no model
		if (!other->GetModel(0).lock()) {
			// Destroy wall
			other->Destroy();
		}
	}

	if (otherCol->type != EECollisionType::BULLET_PLAYER) {
		// Set collide flag
		m_collided = true;
	}
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
