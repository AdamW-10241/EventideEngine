#include "Game/GameObjects/CustomObjects/Grass.h"
#include "Graphics/EMesh.h"
#include "Game/GameObjects/CustomObjects/Floor.h"

void Grass::OnStart()
{	
	// Adjust scale
	GetTransform().scale = glm::vec3(0.15f);
	
	// Add model
	EString modelPath = "Models/Grass/Grass_green.fbx";
	TArray<ESMaterialSlot> materials = {
		ESMaterialSlot::Range(0, 13, ESMaterialDesc{
		{"Models/Grass/textures/Grass_green.png",
		 "Models/Grass/textures/Normal_grass.png"}
		} )
	};
	auto model = LoadModel(modelPath, materials);

	// Place grass randomly on floor mesh
	if (const auto& floor = EGameEngine::GetGameEngine()->FindObjectOfType<Floor>().lock()) {
		PlaceOnFloorRandomly(floor, 25.0f);
	}
}
