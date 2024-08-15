#include "Game/GameObjects/CustomObjects/Floor.h"

void Floor::OnStart()
{
	// Scale floor
	GetTransform().scale = glm::vec3(1.0f);

	// Add model
	auto model = ImportModel("Models/Grid/grid.fbx");

	// Grass base
	auto gridBase = TMakeShared<ETexture>();
	gridBase->LoadTexture("Grid Base", "Models/Grid/textures/grass01.jpg");
	// Grass normal
	auto gridNorm = TMakeShared<ETexture>();
	gridNorm->LoadTexture("Grid Normal", "Models/Grid/textures/grass01_n.jpg");
	// Grass specular
	auto gridSpec = TMakeShared<ETexture>();
	gridSpec->LoadTexture("Grid Normal", "Models/Grid/textures/grass01_s.jpg");

	// Creating materials
	auto gridMat = EGameEngine::GetGameEngine()->CreateMaterial();
	gridMat->m_textureDepth = 10.0f;

	// Assigning textures to the materials
	gridMat->m_baseColourMap = gridBase;
	gridMat->m_normalMap = gridNorm;
	gridMat->m_specularMap = gridSpec;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, gridMat);

	// Add collision
	AddCollision({ GetTransform().position, glm::vec3(100.0f, 1.0f, 100.0f) }, true);
}