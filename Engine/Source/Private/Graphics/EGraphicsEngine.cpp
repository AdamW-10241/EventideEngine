#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EModel.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESCamera.h"
#include "Graphics/ESLight.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

// Test DEBUG model
TWeak<EModel> m_model;

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

	// Add the texture to the mesh if exists
	if (!defaultTexture->LoadTexture("Default Texure", "Textures/T_DefaultGrid.png")) {
		EDebug::Log("Graphics engine default texture did not load.", LT_ERROR);
	}

	// Create the black plastic texture object
	TShared<ETexture> blackPlasticTexture = TMakeShared<ETexture>();

	// Add the texture to the mesh if exists
	if (!blackPlasticTexture->LoadTexture("Black Plastic Texure", "Textures/T_BlackPlastic.png")) {
		EDebug::Log("Graphics engine black plastic texture did not load.", LT_ERROR);
	}

	// Create the black plastic texture object
	TShared<ETexture> coinTexture = TMakeShared<ETexture>();

	// Add the texture to the mesh if exists
	if (!coinTexture->LoadTexture("Coin texture", "Textures/T_Coins.png")) {
		EDebug::Log("Graphics engine coin texture did not load.", LT_ERROR);
	}

	// Log the success of the graphics engine initialisation
	EDebug::Log("Successfully initialised Graphics Engine.", LT_SUCCESS);

	// DEBUG
	m_model = ImportModel("Models/Helmet/Helmet3.fbx");
	m_model.lock()->GetTransform().scale = glm::vec3(0.1f);

	// Add the texture to the model if exists
	TShared<ETexture> tex = TMakeShared<ETexture>();
	tex->LoadTexture("Helmet Base Colour", "Models/Helmet/textures/facetexture_Base_color.jpg");
	TShared<ESMaterial> mat = CreateMaterial();
	mat->m_baseColourMap = tex;

	m_model.lock()->SetMaterialBySlot(0, mat);

	// Create the dir light
	const auto& dirLight = CreateDirLight();

	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 1.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.0f);
		lightRef->intensity = 0.1f;
	}

	// Create the point light
	const auto& pointLight = CreatePointLight();

	if (const auto& lightRef = pointLight.lock()) {
		lightRef->colour = glm::vec3(0.0f, 1.0f, 0.0f);
		lightRef->intensity = 2.0f;
		lightRef->position.x = 20.0f;
	}

	return true;
}

void EGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background color
	glClearColor(0.10f, 0.10f, 0.20f, 1.0f);

	// Clear the back buffer with a solid color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// DEBUG Rotate model
	m_model.lock()->GetTransform().rotation.x += -0.005f;
	m_model.lock()->GetTransform().rotation.y += -0.01f;

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
	const auto& newModel = TMakeShared<EModel>();
	newModel->ImportModel(path);
	m_models.push_back(newModel);
	
	return newModel;
}

TShared<ESMaterial> EGraphicsEngine::CreateMaterial()
{
	return TMakeShared<ESMaterial>();
}
