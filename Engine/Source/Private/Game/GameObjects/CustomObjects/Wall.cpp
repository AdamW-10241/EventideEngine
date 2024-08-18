#include "Game/GameObjects/CustomObjects/Wall.h"
#include "Game/GameObjects/CustomObjects/Floor.h"

#define Super EWorldObject

void Wall::OnStart()
{
	Super::OnStart();

	// Adjust scale
	GetTransform().scale *= glm::vec3(0.1f, 0.07f, 0.1f);

	// Get random direction (L-R or U-D)
	bool direction = EGameEngine::GetGameEngine()->GetRandomIntRange(0, 1) == 0;

	// Get collision size
	glm::vec3 wallCollisionSize = glm::vec3(5.0f, 45.0f, 47.5f);

	// Rotate wall if randomly set
	if (direction) {
		// Rotate transform
		GetTransform().rotation.y += 90;

		float temp = wallCollisionSize.x;
		wallCollisionSize.x = wallCollisionSize.z;
		wallCollisionSize.z = temp;
	}

	// Add a collision
	if (const auto& colRef = AddCollision({ GetTransform().position, wallCollisionSize }, false).lock()) {
		colRef->type = EECollisionType::WALL;
	}

	// Add model
	auto model = ImportModel("Models/Wall/CustomWall.fbx");

	// Wall base
	auto wallBase = TMakeShared<ETexture>();
	wallBase->LoadTexture("Wall Base", "Models/Wall/textures/default_baseColor.png");
	// Wall normal
	auto wallNorm = TMakeShared<ETexture>();
	wallNorm->LoadTexture("Wall Normal", "Models/Wall/textures/default_normal.png");

	// Creating materials
	auto wallMat = EGameEngine::GetGameEngine()->CreateMaterial();
	wallMat->m_specularStrength = 0.0f;

	// Assigning textures to the materials
	wallMat->m_baseColourMap = wallBase;
	wallMat->m_normalMap = wallNorm;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, wallMat);

	// Place randomly on floor mesh
	if (const auto& floor = EGameEngine::GetGameEngine()->FindObjectOfType<Floor>().lock()) {
		PlaceOnFloorRandomly(floor, 25.0f);
	}
}
