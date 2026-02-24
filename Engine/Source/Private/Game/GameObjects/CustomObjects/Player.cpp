#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Graphics/ESLight.h"
#include "Game/GameObjects/EScreenObject.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/euler_angles.hpp>

#define Super Character

Player::Player()
{
	m_health = 10.0f;
	
	m_collided = false;
	m_oldPosition = glm::vec3(0.0f);
	m_weaponBaseOffset = glm::vec3(0.3f, -2.0f, 1.0f);
	m_weaponADSOffset = glm::vec3(0.0f, -1.75f, -2.2f);

	m_leftMouseHeld = false;
	m_rightMouseHeld = false;
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
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f, 10.0f, 5.0f) }, false).lock()) {
		colRef->type = EECollisionType::PLAYER;
	}

	// Add light
	if (const auto& lightRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->CreateSpotLight().lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.7f);
		lightRef->intensity = 20.0f;
		lightRef->outerCutOff = 45.0f;
		lightRef->innerCutOff = 30.0f;
		lightRef->linear *= 1.0f;
		lightRef->quadratic *= 1.0f;

		m_light = lightRef;
	}
}

void Player::OnRegisterInputs(const TShared<EInput>& m_input)
{
	// Mouse pressed
	m_input->OnMousePressed->Bind([this](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			m_leftMouseHeld = true;
		}
		if (button == SDL_BUTTON_RIGHT) {
			m_rightMouseHeld = true;
			m_crosshair->SetDoRender(false);
		}
	});

	// Mouse released
	m_input->OnMouseReleased->Bind([this](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			m_leftMouseHeld = false;
		}
		if (button == SDL_BUTTON_RIGHT) {
			m_rightMouseHeld = false;
			m_crosshair->SetDoRender(true);
		}
	});

	// Key pressed
	m_input->OnKeyPressed->Bind([this](const SDL_Scancode& key) {
		// Toggle light
		if (key == SDL_SCANCODE_F) {
			m_light->ToggleLight();
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
		if (camRef) {
			// If weapon exists
			GetTransform().position = camRef->transform.position;

			// Fire weapon is holding left mouse
			if (m_weapon && m_leftMouseHeld) {
				m_weapon->TryFire(EECollisionType::BULLET_PLAYER, camRef->transform.Forward());
			}
		}
	}
}

void Player::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{	
	Super::OnOverlap(other, col, otherCol);

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

		// Update weapon position
		if (m_weapon) {
			glm::vec3 forward = camRef->transform.Forward();
			glm::vec3 right = camRef->transform.Right();
			glm::vec3 up = camRef->transform.Up();

			glm::vec3 rotatedWeaponOffset =
				forward * m_weaponOffset.z +
				right * m_weaponOffset.x +
				up * m_weaponOffset.y;

			m_weapon->GetTransform().position = camRef->transform.position + rotatedWeaponOffset;
			m_weapon->GetTransform().rotation = glm::vec3(camRef->transform.rotation.x, camRef->transform.rotation.y, 0.0f);
		}

		// Adjust light relative to player
		m_light->position = camRef->transform.position + camRef->transform.Forward() * 5.0f;
		m_light->direction = camRef->transform.Forward();
	}

	// Store old position (old for next loop)
	m_oldPosition = GetTransform().position;

	// Toggle ADS
	m_toBeWeaponOffset = m_rightMouseHeld ? m_weaponADSOffset : m_weaponBaseOffset;
}
