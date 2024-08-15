#include "Game/GameObjects/CustomObjects/Floor.h"
#include "Game/GameObjects/CustomObjects/Grass.h"

void Floor::SpawnGrass()
{
	// Loop
	for (EUi32 i = 0; i < m_grassToSpawn; i++) {
		// Spawn grass
		if (const auto& grass = EGameEngine::GetGameEngine()->CreateObject<Grass>().lock()) {
			// Place on floor randomly
			grass->PlaceOnMeshRandomly(GetModel(0).lock()->GetMesh(0));
		}
	}
}

void Floor::OnStart()
{
	// Scale floor
	GetTransform().scale = glm::vec3(0.02f, 1.0f, 0.02f);

	// Add model
	auto model = ImportModel("Models/Grid/grid.fbx");

	// Grass base
	auto gridBase = TMakeShared<ETexture>();
	gridBase->LoadTexture("Grid Base", "Models/Remains/textures/Ground_baseColor.png");
	// Grass normal
	auto gridNorm = TMakeShared<ETexture>();
	gridNorm->LoadTexture("Grid Normal", "Models/Remains/textures/Ground_normal.png");

	// Creating materials
	auto gridMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	gridMat->m_baseColourMap = gridBase;
	gridMat->m_normalMap = gridNorm;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, gridMat);

	// Add collision
	AddCollision({ GetTransform().position, GetTransform().scale / 2.0f });

	// Spawn grass
	SpawnGrass();
}
