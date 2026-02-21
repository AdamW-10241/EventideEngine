#include "Game/GameObjects/CustomObjects/Skybox.h"

#define Super EWorldObject

void Skybox::OnStart()
{
	Super::OnStart();

	// Load model and textures
	// Scale skybox
	GetTransform().scale = glm::vec3(1.0f);

	// Add model
	EString modelPath = "Models/Skybox/SkyPano_Milkyway.fbx";
	EString texturePath = "Models/Skybox/textures/Milkyway_BG.jpg";
	auto model = LoadModel(modelPath, texturePath);
}
