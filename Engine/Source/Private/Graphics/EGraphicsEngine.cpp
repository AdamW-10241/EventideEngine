#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EModel.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESCamera.h"
#include "Graphics/ESLight.h"
#include "Game/EGameEngine.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

EGraphicsEngine::EGraphicsEngine()
{
	m_sdlGLContext = nullptr;
	m_backgroundColor = EEBackgroundColor::BC_DEFAULT;
}

bool EGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr) {
		EDebug::Log("SDL window was null.", LT_ERROR);
		EDebug::Log("Graphics Engine failed to initialise.", LT_ERROR);
		return false;
	}

	// Create an OpenGL context
	m_sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	// Test if the context failed
	if (m_sdlGLContext == nullptr) {
		EDebug::Log("SDL failed to create GL context: " + std::string(SDL_GetError()), LT_ERROR);
		EDebug::Log("Graphics Engine failed to initialise.", LT_ERROR);
		return false;
	}

	// Make the current context active for the SDL window
	// Test if it failed
	if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {
		EDebug::Log("SDL failed to make GL context current: " + std::string(SDL_GetError()), LT_ERROR);
		EDebug::Log("Graphics Engine failed to initialise.", LT_ERROR);
		return false;
	}

	if (vsync) {
		// Try enable adaptive vsync and test if it failed
		if (SDL_GL_SetSwapInterval(-1) != 0) {
			// Try enable standard vsync and test if it failed
			if (SDL_GL_SetSwapInterval(1) != 0) {
				EDebug::Log(
					"Graphics Engine failed to initialise vsync: " + std::string(SDL_GetError()),
					LT_WARNING);
				return false;
			}
		}
	}

	// Initialise glew
	GLenum glewResult = glewInit();

	// Test if glew failed
	if (glewResult != GLEW_OK) {
		EString errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResult));
		EDebug::Log("Graphics Engine failed to initialise glew: " + errorMsg);
		return false;
	}

	// Enable depth to be tested
	glEnable(GL_DEPTH_TEST);

	// Creater the shader objectss
	m_shader = TMakeShared<EShaderProgram>();

	// Attempt to init shader and test if failed
	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"
	)) {
		EDebug::Log("Graphics engine failed to initialise due to shader failure.");
		return false;
	}

	// Create the camera
	m_camera = TMakeShared<ESCamera>();
	m_camera->transform.position.z -= 5.0f;

	// Create the default texture object
	TShared<ETexture> defaultTexture = TMakeShared<ETexture>();
	if (!defaultTexture->LoadTexture("Default Texure", "Textures/T_DefaultGrid.png")) {
		EDebug::Log("Graphics engine default texture did not load.", LT_ERROR);
	}

	// Log the success of the graphics engine initialisation
	EDebug::Log("Successfully initialised Graphics Engine.", LT_SUCCESS);

	// DEBUG
	// ----------- HELMET
	TWeak<EModel> m_modelHelmet = ImportModel("Models/Helmet/Helmet3.fbx");
	m_modelHelmet.lock()->GetTransform().scale = glm::vec3(0.1f);
	m_modelHelmet.lock()->GetTransform().rotation.y = 90.0f;

	// Face base colour
	TShared<ETexture> helmetTex1 = TMakeShared<ETexture>();
	helmetTex1->LoadTexture("Helmet Face Base Colour", "Models/Helmet/textures/facetexture_Base_color.jpg");
	
	// Face specular
	TShared<ETexture> helmetSpecTex1 = TMakeShared<ETexture>();
	helmetSpecTex1->LoadTexture("Helmet Face Spec Colour", "Models/Helmet/textures/facetexture_Specular.png");

	// Face normal
	TShared<ETexture> helmetNormal1 = TMakeShared<ETexture>();
	helmetNormal1->LoadTexture("Helmet Face Normals", "Models/Helmet/textures/facetexture_Normal_OpenGL.jpg");

	// Head base colour
	TShared<ETexture> helmetTex2 = TMakeShared<ETexture>();
	helmetTex2->LoadTexture("Helmet Head Base Colour", "Models/Helmet/textures/Head_Base_color.jpg");
	
	// Head specular
	TShared<ETexture> helmetSpecTex2 = TMakeShared<ETexture>();
	helmetSpecTex2->LoadTexture("Helmet Head Spec Colour", "Models/Helmet/textures/Head_Specular.png");

	// Head normal
	TShared<ETexture> helmetNormal2 = TMakeShared<ETexture>();
	helmetNormal2->LoadTexture("Helmet Head Normals", "Models/Helmet/textures/Head_Normal_OpenGL.jpg");

	// Creating materials
	TShared<ESMaterial> helmetMat1 = CreateMaterial();
	helmetMat1->m_specularStrength = 1.0f;

	TShared<ESMaterial> helmetMat2 = CreateMaterial();

	// Assigning textures to the materials
	helmetMat1->m_baseColourMap = helmetTex1;
	helmetMat1->m_specularMap = helmetSpecTex1;
	helmetMat1->m_normalMap = helmetNormal1;

	helmetMat2->m_baseColourMap = helmetTex2;
	helmetMat2->m_specularMap = helmetSpecTex2;
	helmetMat2->m_normalMap = helmetNormal2;

	// Setting the material to slots in the model
	m_modelHelmet.lock()->SetMaterialBySlot(0, helmetMat2);
	m_modelHelmet.lock()->SetMaterialBySlot(1, helmetMat1);

	// --------- SKULL
	TWeak<EModel> m_modelSkull = ImportModel("Models/Skull/scene.gltf");
	m_modelSkull.lock()->GetTransform().scale = glm::vec3(0.008f);
	m_modelSkull.lock()->GetTransform().position.x = 1.75f;
	m_modelSkull.lock()->GetTransform().position.y = 0.5f;
	m_modelSkull.lock()->GetTransform().rotation.y = 270.0f;

	// Skull base colour
	TShared<ETexture> skullTex1 = TMakeShared<ETexture>();
	skullTex1->LoadTexture("Skull Base Colour", "Models/Skull/textures/DefaultMaterial_1001_baseColor.jpeg");

	// Skull normal
	TShared<ETexture> skullNormal1 = TMakeShared<ETexture>();
	skullNormal1->LoadTexture("Skull Normal", "Models/Skull/textures/DefaultMaterial_1001_normal.png");

	// Creating materials
	TShared<ESMaterial> skullMat1 = CreateMaterial();

	// Assigning the texture to the base colour map for the material
	skullMat1->m_baseColourMap = skullTex1;
	skullMat1->m_normalMap = skullNormal1;

	// Setting the material to slots in the model
	m_modelSkull.lock()->SetMaterialBySlot(0, skullMat1);

	// --------- REMAINS
	TWeak<EModel> m_modelRemains = ImportModel("Models/Remains/scene.gltf");
	m_modelRemains.lock()->GetTransform().scale = glm::vec3(1.5f);
	m_modelRemains.lock()->GetTransform().position.x = -1.0f;
	m_modelRemains.lock()->GetTransform().position.y = -1.5f;
	m_modelRemains.lock()->GetTransform().position.z = 6.0f;
	m_modelRemains.lock()->GetTransform().rotation.y = 90.0f;

	// Ground base colour
	TShared<ETexture> remainsGroundTexBase = TMakeShared<ETexture>();
	remainsGroundTexBase->LoadTexture("Remains Ground Base Colour", "Models/Remains/textures/Ground_baseColor.png");

	// Ground normal
	TShared<ETexture> remainsGroundTexNormal = TMakeShared<ETexture>();
	remainsGroundTexNormal->LoadTexture("Remains Ground Normal", "Models/Remains/textures/Ground_normal.png");

	// Plant base colour
	TShared<ETexture> remainsPlantTexBase = TMakeShared<ETexture>();
	remainsPlantTexBase->LoadTexture("Remains Plant Base Colour", "Models/Remains/textures/Plants_baseColor.png");

	// Skeleton body base colour
	TShared<ETexture> remainsBodyTexBase = TMakeShared<ETexture>();
	remainsBodyTexBase->LoadTexture("Remains Body Base Colour", "Models/Remains/textures/SkeletonBody_baseColor.jpeg");

	// Skeleton body normal
	TShared<ETexture> remainsBodyTexNormal = TMakeShared<ETexture>();
	remainsBodyTexNormal->LoadTexture("Remains Body Normal", "Models/Remains/textures/SkeletonBody_normal.png");

	// Skeleton limb base colour
	TShared<ETexture> remainsLimbTexBase = TMakeShared<ETexture>();
	remainsLimbTexBase->LoadTexture("Remains Limb Base Colour", "Models/Remains/textures/SkeletonLimb_baseColor.jpeg");

	// Skeleton limb normal
	TShared<ETexture> remainsLimbTexNormal = TMakeShared<ETexture>();
	remainsLimbTexNormal->LoadTexture("Remains Limb Normal", "Models/Remains/textures/SkeletonLimb_normal.png");

	// Skeleton skull base colour
	TShared<ETexture> remainsSkullTexBase = TMakeShared<ETexture>();
	remainsSkullTexBase->LoadTexture("Remains Skull Base Colour", "Models/Remains/textures/SkeletonSkull_baseColor.jpeg");

	// Skeleton skull normal
	TShared<ETexture> remainsSkullTexNormal = TMakeShared<ETexture>();
	remainsSkullTexNormal->LoadTexture("Remains Skull Normal", "Models/Remains/textures/SkeletonSkull_normal.png");

	// Sticks base colour
	TShared<ETexture> remainsStickTexBase = TMakeShared<ETexture>();
	remainsStickTexBase->LoadTexture("Remains Stick Base Colour", "Models/Remains/textures/Sticks_baseColor.png");

	// Stone base colour
	TShared<ETexture> remainsStoneTexBase = TMakeShared<ETexture>();
	remainsStoneTexBase->LoadTexture("Remains Stone Base Colour", "Models/Remains/textures/Stone_baseColor.png");

	// Creating materials
	float brightness = 0.5f;

	TShared<ESMaterial> remainsGroundMat = CreateMaterialB(brightness);
	TShared<ESMaterial> remainsPlantMat = CreateMaterialB(brightness);
	TShared<ESMaterial> remainsBodyMat = CreateMaterialB(brightness);
	TShared<ESMaterial> remainsLimbMat = CreateMaterialB(brightness);
	TShared<ESMaterial> remainsSkullMat = CreateMaterialB(brightness);
	TShared<ESMaterial> remainsSticksMat = CreateMaterialB(brightness);
	TShared<ESMaterial> remainsStonesMat = CreateMaterialB(brightness);

	// Assigning the texture to the materials
	remainsGroundMat->m_baseColourMap = remainsGroundTexBase;
	remainsGroundMat->m_normalMap = remainsGroundTexNormal;
	remainsPlantMat->m_baseColourMap = remainsPlantTexBase;
	remainsBodyMat->m_baseColourMap = remainsBodyTexBase;
	remainsBodyMat->m_normalMap = remainsBodyTexNormal;
	remainsLimbMat->m_baseColourMap = remainsLimbTexBase;
	remainsLimbMat->m_normalMap = remainsLimbTexNormal;
	remainsSkullMat->m_baseColourMap = remainsSkullTexBase;
	remainsSkullMat->m_normalMap = remainsSkullTexNormal;
	remainsSticksMat->m_baseColourMap = remainsStickTexBase;
	remainsStonesMat->m_baseColourMap = remainsStoneTexBase;

	// Setting the material to slots in the model
	m_modelRemains.lock()->SetMaterialBySlot(0, remainsGroundMat);
	m_modelRemains.lock()->SetMaterialBySlot(1, remainsBodyMat);
	m_modelRemains.lock()->SetMaterialBySlot(2, remainsLimbMat);
	m_modelRemains.lock()->SetMaterialBySlot(3, remainsSkullMat);
	m_modelRemains.lock()->SetMaterialBySlot(4, remainsPlantMat);
	m_modelRemains.lock()->SetMaterialBySlot(5, remainsSticksMat);
	m_modelRemains.lock()->SetMaterialBySlot(6, remainsStonesMat);

	// ----------- LIGHTS
	// Create the dir light
	const auto& dirLight = CreateDirLight();
	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 1.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.1f);
		lightRef->intensity = 0.3f;
	}

	// Create the point light
	const auto& pointLight = CreatePointLight();
	if (const auto& lightRef = pointLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.7f);
		lightRef->intensity = 0.3f;
		lightRef->position.x = 0.0f;
	}

	return true;
}

void EGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background color
	ESBackgroundColorData backgroundColor = backgroundColorDataV.at(m_backgroundColor);
	glClearColor(backgroundColor.m_color[0], backgroundColor.m_color[1], backgroundColor.m_color[2], 1.0f);

	// Clear the back buffer with a solid color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// DEBUG Rotate model
	float rotationRate = 10.0f;
	m_models.at(0)->GetTransform().rotation.y += EGameEngine::GetGameEngine()->DeltaTimeF() * rotationRate;

	static float lightTimer = 0.0f;
	const float timeToRotate = 1.0f;
	lightTimer += EGameEngine::GetGameEngine()->DeltaTimeF();

	if (lightTimer > timeToRotate)
		lightTimer = 0.0f;

	if (const auto& lightRef = std::dynamic_pointer_cast<ESPointLight>(m_lights[1])) {
		lightRef->position.x = sin(lightTimer / timeToRotate * 2 * PI) * 2.0f;
		lightRef->position.z = cos(lightTimer / timeToRotate * 2 * PI) * 2.0f;
	}

	// Activate shader
	m_shader->Activate();

	// Set the world transformations based on the camera
	m_shader->SetWorldTransform(m_camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	for (const auto& modelRef : m_models) {
		modelRef->Render(m_shader, m_lights);
	}

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}

TWeak<ESPointLight> EGraphicsEngine::CreatePointLight()
{
	const auto& newLight = TMakeShared<ESPointLight>();
	m_lights.push_back(newLight);
	
	return newLight;
}

TWeak<ESDirLight> EGraphicsEngine::CreateDirLight()
{
	const auto& newLight = TMakeShared<ESDirLight>();
	m_lights.push_back(newLight);

	return newLight;
}

TWeak<EModel> EGraphicsEngine::ImportModel(const EString& path)
{
	// Get spawn id
	size_t spawnID = m_models.size();

	// Create model
	const auto& newModel = TMakeShared<EModel>(spawnID, path);
	newModel->ImportModel(path);
	m_models.push_back(newModel);
	
	return newModel;
}

TShared<ESMaterial> EGraphicsEngine::CreateMaterial()
{
	return TMakeShared<ESMaterial>();
}

TShared<ESMaterial> EGraphicsEngine::CreateMaterialB(float brightness)
{
	TShared<ESMaterial> material = CreateMaterial();
	material->m_brightness = brightness;
	return material;
}

void EGraphicsEngine::AdjustTextureDepth(float delta)
{
	// Adjust the texture depth by the delta
	m_shader->AdjustTextureDepth(delta);
}

void EGraphicsEngine::ResetTextureDepth()
{
	// Reset the texture depth
	m_shader->ResetTextureDepth();
}