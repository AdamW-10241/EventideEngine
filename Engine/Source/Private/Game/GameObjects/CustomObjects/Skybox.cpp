#include "Game/GameObjects/CustomObjects/Skybox.h"

#define Super EWorldObject

void Skybox::OnStart()
{
	Super::OnStart();

	// Load model and textures
	// Scale skybox
	GetTransform().scale = glm::vec3(1.0f);

	// Add model
	auto model = ImportModel("Models/Skybox/SkyPano_Milkyway.fbx");

	// Skybox base
	auto skyboxBase = TMakeShared<ETexture>();
	skyboxBase->LoadTexture("Skybox Base", "Models/Skybox/textures/Milkyway_BG.jpg");

	// Creating materials
	auto skyboxMat = EGameEngine::GetGameEngine()->CreateMaterial();
	skyboxMat->m_specularStrength = 0.0f;

	// Assigning textures to the materials
	skyboxMat->m_baseColourMap = skyboxBase;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, skyboxMat);
}
