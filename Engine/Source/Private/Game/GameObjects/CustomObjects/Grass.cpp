#include "Game/GameObjects/CustomObjects/Grass.h"
#include "Graphics/EModel.h"

Grass::Grass(TWeak<EModel>& modelToPlaceOn) { PositionRandomlyOnMesh(modelToPlaceOn); }

void Grass::OnStart()
{
	// Add model
	auto model = ImportModel("Models/Grass/Grass_green.fbx");

	// Grass base
	auto grassBase = TMakeShared<ETexture>();
	grassBase->LoadTexture("Grass Base", "Models/Grass/textures/Grass_green.png");
	// Grass normal
	auto grassNorm = TMakeShared<ETexture>();
	grassNorm->LoadTexture("Grass Normal", "Models/Grass/textures/Normal_grass.png");

	// Creating materials
	auto grassMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	grassMat->m_baseColourMap = grassBase;
	grassMat->m_normalMap = grassNorm;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, grassMat);

	// Adjust size
	// model.lock()->GetTransform().scale = glm::vec3(1.0f);
}

void Grass::PositionRandomlyOnMesh(TWeak<EModel>& modelToPlaceOn)
{
	// Get a random position from the first mesh in the stack
	glm::vec3 randomPositionOnModel = modelToPlaceOn.lock()->GetMeshStack().at(0)->GetRandomVertexPosition();
	GetTransform().position = randomPositionOnModel;
}
