#include "Game/GameObjects/CustomObjects/Coin.h"

#define Super EWorldObject

void Coin::OnStart()
{
	Super::OnStart();

	// Scale mode
	GetTransform().scale = glm::vec3(200.0f);

	// Add model
	auto model = ImportModel("Models/Coin/scene.gltf");

	// Coin base
	auto coinBase = TMakeShared<ETexture>();
	coinBase->LoadTexture("Coin Base", "Models/Coin/textures/TB_coin_mat_baseColor.jpeg");
	// Coin normal
	auto coinNorm = TMakeShared<ETexture>();
	coinNorm->LoadTexture("Coin Normal", "Models/Coin/textures/TB_coin_mat_normal.jpeg");

	// Creating materials
	auto coinMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	coinMat->m_baseColourMap = coinBase;
	coinMat->m_normalMap = coinNorm;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, coinMat);

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
		// Increment points
		EGameEngine::GetGameEngine()->GetPoints() += m_points;

		// Display points
		EString pointAmountText = toEString(EGameEngine::GetGameEngine()->GetPoints() * 1000);
		EDebug::Log("Points: " + pointAmountText);

		// Destroy coin
		Destroy();
	}
}
