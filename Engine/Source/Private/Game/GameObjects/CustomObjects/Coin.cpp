#include "Game/GameObjects/CustomObjects/Coin.h"
#include "Graphics/EText.h"
#include "Game/GameObjects/EScreenObject.h"

#define Super EWorldObject

void Coin::OnStart()
{
	Super::OnStart();

	// Scale mode
	GetTransform().scale = glm::vec3(200.0f);

	// Add model
	EString modelPath = "Models/Coin/scene.gltf";
	TArray<ESMaterialSlot> materials = { 
		{ {0}, ESMaterialDesc{
			{"Models/Coin/textures/TB_coin_mat_baseColor.jpeg", 
			"Models/Coin/textures/TB_coin_mat_normal.jpeg"}
		} } 
	};
	auto model = LoadModel(modelPath, materials);

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(8.0f) }, false).lock()) {
		colRef->type = EECollisionType::COLLECTABLE;
	}
}

void Coin::OnTick(float deltaTime)
{
	Super::OnTick(deltaTime);
	
	// Rotate
	float rotationSpeed = 60.0f;
	GetTransform().rotation.y += deltaTime * rotationSpeed;
}

void Coin::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{
	Super::OnOverlap(other, col, otherCol);
	
	if (otherCol->type == EECollisionType::PLAYER) {
		EGameEngine::GetGameEngine()->AddPoints(1000, true);
		if (auto soundManager = EGameEngine::GetGameEngine()->GetSoundManager().lock()) {
			soundManager->PlaySound(EE_SOUND_COIN, 100);
		}
		Destroy();
	}
}
