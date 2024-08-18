#include "Game/GameObjects/CustomObjects/Grass.h"
#include "Graphics/EMesh.h"
#include "Game/GameObjects/CustomObjects/Floor.h"

void Grass::OnStart()
{	
	// Adjust scale
	GetTransform().scale = glm::vec3(0.2f);
	
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
	model.lock()->SetMaterialBySlot(1, grassMat);
	model.lock()->SetMaterialBySlot(2, grassMat);
	model.lock()->SetMaterialBySlot(3, grassMat);
	model.lock()->SetMaterialBySlot(4, grassMat);
	model.lock()->SetMaterialBySlot(5, grassMat);
	model.lock()->SetMaterialBySlot(6, grassMat);
	model.lock()->SetMaterialBySlot(7, grassMat);
	model.lock()->SetMaterialBySlot(8, grassMat);
	model.lock()->SetMaterialBySlot(9, grassMat);
	model.lock()->SetMaterialBySlot(10, grassMat);
	model.lock()->SetMaterialBySlot(11, grassMat);
	model.lock()->SetMaterialBySlot(12, grassMat);
	model.lock()->SetMaterialBySlot(13, grassMat);

	// Place grass randomly on floor mesh
	if (const auto& floor = EGameEngine::GetGameEngine()->FindObjectOfType<Floor>().lock()) {
		PlaceOnFloorRandomly(floor, 25.0f);
	}
}
