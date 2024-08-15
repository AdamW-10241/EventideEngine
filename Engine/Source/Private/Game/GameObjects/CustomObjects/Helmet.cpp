#include "Game/GameObjects/CustomObjects/Helmet.h"
#include "Graphics/EModel.h"

void Helmet::OnStart()
{
	// Add model
	auto model = ImportModel("Models/Helmet/Helmet3.fbx");

	// Face base
	auto faceBase = TMakeShared<ETexture>();
	faceBase->LoadTexture("Helmet Face Base", "Models/Helmet/textures/facetexture_Base_color.jpg");
	// Face specular
	auto faceSpec = TMakeShared<ETexture>();
	faceSpec->LoadTexture("Helmet Face Specular", "Models/Helmet/textures/facetexture_Specular.png");
	// Face normal
	auto faceNorm = TMakeShared<ETexture>();
	faceNorm->LoadTexture("Helmet Face Normal", "Models/Helmet/textures/facetexture_Normal_OpenGL.jpg");

	// Head base
	auto headBase = TMakeShared<ETexture>();
	headBase->LoadTexture("Helmet Head Base", "Models/Helmet/textures/Head_Base_color.jpg");
	// Head specular
	auto headSpec = TMakeShared<ETexture>();
	headSpec->LoadTexture("Helmet Head Specular", "Models/Helmet/textures/Head_Specular.png");
	// Head normal
	auto headNorm = TMakeShared<ETexture>();
	headNorm->LoadTexture("Helmet Head Normal", "Models/Helmet/textures/Head_Normal_OpenGL.jpg");

	// Creating materials
	auto faceMat = EGameEngine::GetGameEngine()->CreateMaterial();
	faceMat->m_specularStrength = 1.0f;
	auto headMat = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning textures to the materials
	faceMat->m_baseColourMap = faceBase;
	faceMat->m_specularMap = faceSpec;
	faceMat->m_normalMap = faceNorm;

	headMat->m_baseColourMap = headBase;
	headMat->m_specularMap = headSpec;
	headMat->m_normalMap = headNorm;

	// Add materials to the model
	model.lock()->SetMaterialBySlot(0, headMat);
	model.lock()->SetMaterialBySlot(1, faceMat);

	// Add collision
	AddCollision({ GetTransform().position, glm::vec3(7.0f) }, true);
}

void Helmet::OnTick(float deltaTime)
{
	// Rotate
	float rotationSpeed = 60.0f;
	GetTransform().rotation.y += deltaTime * rotationSpeed;
}

void Helmet::OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, const TShared<ESCollision>& otherCol)
{
	if (otherCol->type == EECollisionType::PLAYER) {
		col->debugColour = glm::vec3(1.0f, 0.0f, 0.0f);
	}
}
