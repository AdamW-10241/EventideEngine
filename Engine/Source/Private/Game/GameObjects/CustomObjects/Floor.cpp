#include "Game/GameObjects/CustomObjects/Floor.h"

void Floor::OnStart()
{
	// Scale floor
	GetTransform().scale = glm::vec3(0.3f);

	// Add model
	EString modelPath = "Models/Grid/grid.fbx";
	TArray<ESMaterialSlot> materials = {
		{ {0}, ESMaterialDesc{
			{"Models/Grid/textures/grass01.jpg",
			 "Models/Grid/textures/grass01_n.jpg"}
		}.withTextureDepth(10.0f)}
	};
	auto model = LoadModel(modelPath, materials);

	// Add collision
	AddCollision({ GetTransform().position, glm::vec3(300.0f, 1.0f, 300.0f) }, false);
}