#include "Game/GameObjects/CustomObjects/Floor.h"

void Floor::OnStart()
{
	// Scale floor
	GetTransform().scale = glm::vec3(0.3f);

	// Add model
	EString modelPath = "Models/Grid/grid.fbx";
	EString texturePath = "Models/Grid/textures/grass01.jpg";
	auto model = LoadModel(modelPath, texturePath);
	//gridNorm->LoadTexture("Grid Normal", "Models/Grid/textures/grass01_n.jpg");
	//gridSpec->LoadTexture("Grid Normal", "Models/Grid/textures/grass01_s.jpg");

	// Add collision
	AddCollision({ GetTransform().position, glm::vec3(300.0f, 1.0f, 300.0f) }, false);
}