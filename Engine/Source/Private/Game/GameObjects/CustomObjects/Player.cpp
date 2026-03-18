#include "Game/GameObjects/CustomObjects/Player.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/ESCamera.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Graphics/ESLight.h"
#include "Game/GameObjects/EScreenObject.h"
#include "Game/GameObjects/CustomObjects/GUIHUD.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/euler_angles.hpp>

#define Super Character

Player::Player(glm::vec3 spawnLocation)
{
	m_maxHealth = 10.0f;
	ResetHealth();
	m_healRate = 1.0f;
	
	m_collided = false;
	m_oldPosition = glm::vec3(0.0f);
	m_weaponBaseOffset = glm::vec3(0.3f, -2.0f, 1.0f);
	m_weaponADSOffset = glm::vec3(0.0f, -1.75f, -2.2f);

	m_leftMouseHeld = false;
	m_rightMouseHeld = false;

	SetSpawnLocation(spawnLocation);
}

void Player::SetSpawnLocation(glm::vec3 position)
{
	if (auto camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		camRef->transform.position = position;
	}
}

void Player::OnStart()
{
	Super::OnStart();

	// If camera exists,
	if (auto camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		// Move to camera
		GetTransform().position = camRef->transform.position;
		// Set old position (for next loop)
		m_oldPosition = GetTransform().position;
	}

	// Add a collision
	if (auto colRef = AddCollision({ GetTransform().position, glm::vec3(5.0f, 10.0f, 5.0f) }, false).lock()) {
		colRef->type = EECollisionType::PLAYER;
	}

	// Add light
	if (auto lightRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->CreateSpotLight().lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.7f);
		lightRef->intensity = 20.0f;
		lightRef->outerCutOff = 45.0f;
		lightRef->innerCutOff = 30.0f;
		lightRef->linear *= 1.0f;
		lightRef->quadratic *= 1.0f;

		m_light = lightRef;
	}

	// Add weapon
	if (auto weapon = EGameEngine::GetGameEngine()->CreateObject<Weapon>(GetSharedRef<Player>(), true, 1.0f, 1000.0f, 0.1f, false).lock()) {
		AddWeapon(weapon);
	}
	
	// Get HUD
	auto hud = EGameEngine::GetGameEngine()->GetGameHUD().lock();
	if (!hud) { 
		EDebug::Log("Game HUD could not be locked.\n");
		return;
	}

	// Add crosshair
	if (auto crosshair = hud->AddScreenObject().lock()) {
		if (auto window = EGameEngine::GetGameEngine()->GetWindow().lock()) {
			// Add sprite
			ESAddSpriteConfig config{ .texturePath = "Sprites/Crosshairs/crosshair009.png", .screenPositionRatio = {0.5, 0.5} };
			if (auto spriteRef = crosshair->AddSprite(config).lock()) {
				spriteRef->GetTransform().scale *= 0.4f;
				spriteRef->GetTransform().CenterOnPosition();
			}
			// Add to player
			AddCrosshair(crosshair);
		}
	}
}

void Player::OnRegisterInputs(const TShared<EInput>& m_input)
{
	// Mouse pressed
	BIND_INPUT(m_input, OnMousePressed, [this](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			m_leftMouseHeld = true;
		}
		if (button == SDL_BUTTON_RIGHT) {
			m_rightMouseHeld = true;
			if (auto crosshair = m_crosshair.lock()) crosshair->SetDoRender(false);
		}
	});

	// Mouse released
	BIND_INPUT(m_input, OnMouseReleased, [this](const EUi8& button) {
		if (button == SDL_BUTTON_LEFT) {
			m_leftMouseHeld = false;
		}
		if (button == SDL_BUTTON_RIGHT) {
			m_rightMouseHeld = false;
			if (auto crosshair = m_crosshair.lock()) crosshair->SetDoRender(true);
		}
	});

	// Key pressed
	BIND_INPUT(m_input, OnKeyPressed, [this](const SDL_Scancode& key) {
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
	if (auto camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		GetTransform().position = camRef->transform.position;
	}
}

void Player::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{	
	Super::OnOverlap(other, col, otherCol);

	if (otherCol->type != EECollisionType::BULLET_PLAYER) {
		// Set collide flag
		m_collided = true;

		if (otherCol->type == EECollisionType::BULLET_ENEMY) {
			if (auto soundManager = EGameEngine::GetGameEngine()->GetSoundManager().lock()) {
				soundManager->PlaySound(EE_SOUND_HIT_PLAYER);
			}

			// Debug HIT notif
			//EString text = "Player Hit!";
			//glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
			//EGameEngine::GetGameEngine()->AddTextNotif(text, color);
		}
	}
}

void Player::OnPostTick(float deltaTime)
{
	Super::OnPostTick(deltaTime);

	// Toggle ADS
	m_toBeWeaponOffset = m_rightMouseHeld ? m_weaponADSOffset : m_weaponBaseOffset;

	// Move to camera, or reverse frame camera movement, based on collisions
	if (auto camRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()) {
		if (m_collided) {
			// Reset camera position to before collision
			camRef->transform.position = m_oldPosition;
			// Reset player position to before collision
			GetTransform().position = m_oldPosition;
		}

		// Update weapon
		if (auto weapon = m_weapon.lock()) {
			// Move with player
			glm::vec3 forward = camRef->transform.Forward();
			glm::vec3 right = camRef->transform.Right();
			glm::vec3 up = camRef->transform.Up();

			glm::vec3 rotatedWeaponOffset =
				forward * m_weaponOffset.z +
				right * m_weaponOffset.x +
				up * m_weaponOffset.y;

			weapon->GetTransform().position = camRef->transform.position + rotatedWeaponOffset;
			weapon->GetTransform().rotation = glm::vec3(camRef->transform.rotation.x, camRef->transform.rotation.y, 0.0f);

			// Fire weapon if holding left mouse
			if (auto weapon = m_weapon.lock()) {
				if (m_leftMouseHeld) {
					weapon->TryFire(EECollisionType::BULLET_PLAYER, camRef->transform.Forward());
				}
			}
		}

		// Adjust light relative to player
		m_light->position = camRef->transform.position + camRef->transform.Forward() * 5.0f;
		m_light->direction = camRef->transform.Forward();
	}

	// Store old position (old for next loop)
	m_oldPosition = GetTransform().position;
}
