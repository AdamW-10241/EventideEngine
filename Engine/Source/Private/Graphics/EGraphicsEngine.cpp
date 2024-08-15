#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EModel.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESCamera.h"
#include "Graphics/ESLight.h"
#include "Game/EGameEngine.h"
#include "Math/ESCollision.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

// Collision cube vertices
const std::vector<ESVertexData> colMeshVData = {
//       x      y      z 
	{ {	-1.0f, -1.0f,  1.0f } }, // bl f
	{ {	 1.0f, -1.0f,  1.0f } }, // br f
	{ {  1.0f,  1.0f,  1.0f } }, // tr f
	{ { -1.0f,  1.0f,  1.0f } }, // tl f
	{ {	-1.0f, -1.0f, -1.0f } }, // bl b
	{ {	 1.0f, -1.0f, -1.0f } }, // br b
	{ {  1.0f,  1.0f, -1.0f } }, // tr b
	{ { -1.0f,  1.0f, -1.0f } }, // tl b
};

// Collision cube indices
const std::vector<EUi32> colMeshIData = {
	0, 1, 1, 2, 2, 3, 3, 0, // front
	4, 5, 5, 6, 6, 7, 7, 4, // back
	0, 4, 1, 5, 2, 6, 3, 7  // sides
};

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

	// Create the shader object
	m_shader = TMakeShared<EShaderProgram>();

	// Attempt to init shader and test if failed
	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"
	)) {
		EDebug::Log("Graphics engine failed to initialise due to shader failure.");
		return false;
	}

	// Creater the wire shader object
	m_wireShader = TMakeShared<EShaderProgram>();

	// Attempt to init shader and test if failed
	if (!m_wireShader->InitShader(
		"Shaders/Wireframe/Wireframe.vertex",
		"Shaders/Wireframe/Wireframe.frag"
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

	// Init a default material for all models
	m_defaultMaterial = TMakeShared<ESMaterial>();
	m_defaultMaterial->m_baseColourMap = defaultTexture;

	// Log the success of the graphics engine initialisation
	EDebug::Log("Successfully initialised Graphics Engine.", LT_SUCCESS);

	// DEBUG
	// --------- SKULL
	//TWeak<EModel> m_modelSkull = ImportModel("Models/Skull/scene.gltf");
	//m_modelSkull.lock()->GetTransform().scale = glm::vec3(0.008f);
	//m_modelSkull.lock()->GetTransform().position.x = 1.75f;
	//m_modelSkull.lock()->GetTransform().position.y = 0.5f;
	//m_modelSkull.lock()->GetTransform().rotation.y = 270.0f;

	//// Skull base colour
	//TShared<ETexture> skullTex1 = TMakeShared<ETexture>();
	//skullTex1->LoadTexture("Skull Base Colour", "Models/Skull/textures/DefaultMaterial_1001_baseColor.jpeg");

	//// Skull normal
	//TShared<ETexture> skullNormal1 = TMakeShared<ETexture>();
	//skullNormal1->LoadTexture("Skull Normal", "Models/Skull/textures/DefaultMaterial_1001_normal.png");

	//// Creating materials
	//TShared<ESMaterial> skullMat1 = CreateMaterial();

	//// Assigning the texture to the base colour map for the material
	//skullMat1->m_baseColourMap = skullTex1;
	//skullMat1->m_normalMap = skullNormal1;

	//// Setting the material to slots in the model
	//m_modelSkull.lock()->SetMaterialBySlot(0, skullMat1);

	//// --------- REMAINS
	//TWeak<EModel> m_modelRemains = ImportModel("Models/Remains/scene.gltf");
	//m_modelRemains.lock()->GetTransform().scale = glm::vec3(1.5f);
	//m_modelRemains.lock()->GetTransform().position.x = -1.0f;
	//m_modelRemains.lock()->GetTransform().position.y = -1.5f;
	//m_modelRemains.lock()->GetTransform().position.z = 6.0f;
	//m_modelRemains.lock()->GetTransform().rotation.y = 90.0f;

	//// Ground base colour
	//TShared<ETexture> remainsGroundTexBase = TMakeShared<ETexture>();
	//remainsGroundTexBase->LoadTexture("Remains Ground Base Colour", "Models/Remains/textures/Ground_baseColor.png");

	//// Ground normal
	//TShared<ETexture> remainsGroundTexNormal = TMakeShared<ETexture>();
	//remainsGroundTexNormal->LoadTexture("Remains Ground Normal", "Models/Remains/textures/Ground_normal.png");

	//// Plant base colour
	//TShared<ETexture> remainsPlantTexBase = TMakeShared<ETexture>();
	//remainsPlantTexBase->LoadTexture("Remains Plant Base Colour", "Models/Remains/textures/Plants_baseColor.png");

	//// Skeleton body base colour
	//TShared<ETexture> remainsBodyTexBase = TMakeShared<ETexture>();
	//remainsBodyTexBase->LoadTexture("Remains Body Base Colour", "Models/Remains/textures/SkeletonBody_baseColor.jpeg");

	//// Skeleton body normal
	//TShared<ETexture> remainsBodyTexNormal = TMakeShared<ETexture>();
	//remainsBodyTexNormal->LoadTexture("Remains Body Normal", "Models/Remains/textures/SkeletonBody_normal.png");

	//// Skeleton limb base colour
	//TShared<ETexture> remainsLimbTexBase = TMakeShared<ETexture>();
	//remainsLimbTexBase->LoadTexture("Remains Limb Base Colour", "Models/Remains/textures/SkeletonLimb_baseColor.jpeg");

	//// Skeleton limb normal
	//TShared<ETexture> remainsLimbTexNormal = TMakeShared<ETexture>();
	//remainsLimbTexNormal->LoadTexture("Remains Limb Normal", "Models/Remains/textures/SkeletonLimb_normal.png");

	//// Skeleton skull base colour
	//TShared<ETexture> remainsSkullTexBase = TMakeShared<ETexture>();
	//remainsSkullTexBase->LoadTexture("Remains Skull Base Colour", "Models/Remains/textures/SkeletonSkull_baseColor.jpeg");

	//// Skeleton skull normal
	//TShared<ETexture> remainsSkullTexNormal = TMakeShared<ETexture>();
	//remainsSkullTexNormal->LoadTexture("Remains Skull Normal", "Models/Remains/textures/SkeletonSkull_normal.png");

	//// Sticks base colour
	//TShared<ETexture> remainsStickTexBase = TMakeShared<ETexture>();
	//remainsStickTexBase->LoadTexture("Remains Stick Base Colour", "Models/Remains/textures/Sticks_baseColor.png");

	//// Stone base colour
	//TShared<ETexture> remainsStoneTexBase = TMakeShared<ETexture>();
	//remainsStoneTexBase->LoadTexture("Remains Stone Base Colour", "Models/Remains/textures/Stone_baseColor.png");

	//// Creating materials
	//float brightness = 0.5f;

	//TShared<ESMaterial> remainsGroundMat = CreateMaterialB(brightness);
	//TShared<ESMaterial> remainsPlantMat = CreateMaterialB(brightness);
	//TShared<ESMaterial> remainsBodyMat = CreateMaterialB(brightness);
	//TShared<ESMaterial> remainsLimbMat = CreateMaterialB(brightness);
	//TShared<ESMaterial> remainsSkullMat = CreateMaterialB(brightness);
	//TShared<ESMaterial> remainsSticksMat = CreateMaterialB(brightness);
	//TShared<ESMaterial> remainsStonesMat = CreateMaterialB(brightness);

	//// Assigning the texture to the materials
	//remainsGroundMat->m_baseColourMap = remainsGroundTexBase;
	//remainsGroundMat->m_normalMap = remainsGroundTexNormal;
	//remainsPlantMat->m_baseColourMap = remainsPlantTexBase;
	//remainsBodyMat->m_baseColourMap = remainsBodyTexBase;
	//remainsBodyMat->m_normalMap = remainsBodyTexNormal;
	//remainsLimbMat->m_baseColourMap = remainsLimbTexBase;
	//remainsLimbMat->m_normalMap = remainsLimbTexNormal;
	//remainsSkullMat->m_baseColourMap = remainsSkullTexBase;
	//remainsSkullMat->m_normalMap = remainsSkullTexNormal;
	//remainsSticksMat->m_baseColourMap = remainsStickTexBase;
	//remainsStonesMat->m_baseColourMap = remainsStoneTexBase;

	//// Setting the material to slots in the model
	//m_modelRemains.lock()->SetMaterialBySlot(0, remainsGroundMat);
	//m_modelRemains.lock()->SetMaterialBySlot(1, remainsBodyMat);
	//m_modelRemains.lock()->SetMaterialBySlot(2, remainsLimbMat);
	//m_modelRemains.lock()->SetMaterialBySlot(3, remainsSkullMat);
	//m_modelRemains.lock()->SetMaterialBySlot(4, remainsPlantMat);
	//m_modelRemains.lock()->SetMaterialBySlot(5, remainsSticksMat);
	//m_modelRemains.lock()->SetMaterialBySlot(6, remainsStonesMat);

	// ----------- LIGHTS
	// Create the dir light
	const auto& dirLight = CreateDirLight();
	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 1.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.5f);
		lightRef->intensity = 0.3f;
	}

	// Create the point light
	const auto& pointLight = CreatePointLight();
	if (const auto& lightRef = pointLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.7f);
		lightRef->intensity = 1.0f;
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

	// DEBUG Rotate light
	static float lightTimer = 0.0f;
	const float timeToRotate = 1.0f;
	lightTimer += EGameEngine::GetGameEngine()->DeltaTimeF();

	if (lightTimer > timeToRotate)
		lightTimer = 0.0f;

	if (const auto& lightRef = std::dynamic_pointer_cast<ESPointLight>(m_lights[1])) {
		lightRef->position.x = sin(lightTimer / timeToRotate * 2 * PI) * 2.0f;
		lightRef->position.z = cos(lightTimer / timeToRotate * 2 * PI) * 2.0f;
	}

	// ---------- NORMAL SHADER
	// Activate shader
	m_shader->Activate();

	// Set the world transformations based on the camera
	m_shader->SetWorldTransform(m_camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	for (int i = m_models.size() - 1; i >= 0; --i) {
		// Detecting if the reference still exists
		if (const auto& modelRef = m_models[i].lock()) {
			// Render if there is a reference
			modelRef->Render(m_shader, m_lights);
		}
		else {
			// Erase from the array if there is no reference
			m_models.erase(m_models.begin() + i);
		}
	}

	// ---------- WIRE SHADER
	if (m_collisions.size() > 0) {
		// Activate shader
		m_wireShader->Activate();

		// Set the world transformations based on the camera
		m_wireShader->SetWorldTransform(m_camera);

		// Render custom graphics
		for (int i = m_collisions.size() - 1; i >= 0; --i) {
			// Detecting if the reference still exists
			if (const auto& colRef = m_collisions[i].lock()) {
				// Convert position of collision into transform'
				ESTransform transform;
				transform.position = colRef->box.position;
				transform.scale = colRef->box.halfSize;

				// Set the colour of the collision
				m_wireShader->SetWireColour(colRef->debugColour);

				// Render if there is a reference
				colRef->debugMesh->WireRender(m_wireShader, transform);
			}
			else {
				// Erase from the array if there is no reference
				m_collisions.erase(m_collisions.begin() + i);
			}
		}
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

TShared<EModel> EGraphicsEngine::ImportModel(const EString& path)
{
	// Get spawn id
	size_t spawnID = m_models.size();

	// Create model
	const auto& newModel = TMakeShared<EModel>(spawnID, path);
	newModel->ImportModel(path, m_defaultMaterial);
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

void EGraphicsEngine::CreateCollisionMesh(const TWeak<ESCollision>& col)
{
	if (const auto& colRef = col.lock()) {
		TShared<EMesh> newMesh = TMakeShared<EMesh>();

		// Create a box of lines
		newMesh->CreateMesh(colMeshVData, colMeshIData);

		// Store the shared mesh into the collision
		colRef->debugMesh = newMesh;

		// Add the collision to the stack
		m_collisions.push_back(col);
	}
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