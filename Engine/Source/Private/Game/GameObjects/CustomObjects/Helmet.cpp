#include "Game/GameObjects/CustomObjects/Helmet.h"
#include "Graphics/EModel.h"

Helmet::Helmet()
{
	// ----------- HELMET
	if (const auto& modelRef = ImportModel("Models/Helmet/Helmet3.fbx").lock()) {
		modelRef->m_offset.position.x = 10.0f;
	}

	//.lock()->GetTransform().scale = glm::vec3(0.1f);
	//m_modelHelmet.lock()->GetTransform().rotation.y = 90.0f;
	//m_
	//// Face base colour
	//TShared<ETexture> helmetTex1 = TMakeShared<ETexture>();
	//helmetTex1->LoadTexture("Helmet Face Base Colour", "Models/Helmet/textures/facetexture_Base_color.jpg");

	//// Face specular
	//TShared<ETexture> helmetSpecTex1 = TMakeShared<ETexture>();
	//helmetSpecTex1->LoadTexture("Helmet Face Spec Colour", "Models/Helmet/textures/facetexture_Specular.png");

	//// Face normal
	//TShared<ETexture> helmetNormal1 = TMakeShared<ETexture>();
	//helmetNormal1->LoadTexture("Helmet Face Normals", "Models/Helmet/textures/facetexture_Normal_OpenGL.jpg");

	//// Head base colour
	//TShared<ETexture> helmetTex2 = TMakeShared<ETexture>();
	//helmetTex2->LoadTexture("Helmet Head Base Colour", "Models/Helmet/textures/Head_Base_color.jpg");

	//// Head specular
	//TShared<ETexture> helmetSpecTex2 = TMakeShared<ETexture>();
	//helmetSpecTex2->LoadTexture("Helmet Head Spec Colour", "Models/Helmet/textures/Head_Specular.png");

	//// Head normal
	//TShared<ETexture> helmetNormal2 = TMakeShared<ETexture>();
	//helmetNormal2->LoadTexture("Helmet Head Normals", "Models/Helmet/textures/Head_Normal_OpenGL.jpg");

	//// Creating materials
	//TShared<ESMaterial> helmetMat1 = CreateMaterial();
	//helmetMat1->m_specularStrength = 1.0f;

	//TShared<ESMaterial> helmetMat2 = CreateMaterial();

	//// Assigning textures to the materials
	//helmetMat1->m_baseColourMap = helmetTex1;
	//helmetMat1->m_specularMap = helmetSpecTex1;
	//helmetMat1->m_normalMap = helmetNormal1;

	//helmetMat2->m_baseColourMap = helmetTex2;
	//helmetMat2->m_specularMap = helmetSpecTex2;
	//helmetMat2->m_normalMap = helmetNormal2;

	//// Setting the material to slots in the model
	//m_modelHelmet.lock()->SetMaterialBySlot(0, helmetMat2);
	//m_modelHelmet.lock()->SetMaterialBySlot(1, helmetMat1);
}
