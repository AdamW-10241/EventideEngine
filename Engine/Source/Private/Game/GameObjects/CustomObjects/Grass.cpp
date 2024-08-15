#include "Game/GameObjects/CustomObjects/Grass.h"
#include "Graphics/EMesh.h"

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

void Grass::PlaceOnMeshRandomly(TUnique<EMesh>& meshToPlaceOn)
{
	// Set the position to a random vertex position on the mesh
	GetTransform().position += meshToPlaceOn->GetRandomVertexPosition();
}
