#include "Game/GameObjects/CustomObjects/Coin.h"

#define Super EWorldObject

void Coin::OnStart()
{
	Super::OnStart();

	// Scale mode
	GetTransform().scale = glm::vec3(2.0f);

	// Add model
	auto model = ImportModel("Models/Coin/model.gltf");

	// Coin base
	auto coinBase = TMakeShared<ETexture>();
	coinBase->LoadTexture("Coin Base", "Textures/T_Coins.png");

	// Creating materials
	auto coinMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	coinMat->m_baseColourMap = coinBase;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, coinMat);

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(8.0f) }, true).lock()) {
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
		EGameEngine::GetGameEngine()->GetPoints() += m_points;
		EDebug::Log("Points: " + toEString(EGameEngine::GetGameEngine()->GetPoints()));
		Destroy();
	}
}
