#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EModel.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESCamera.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <random>

// Initialise a random generator
std::default_random_engine RandGenerator;

#define SPIKE_NUM 20

bool EGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	// Set the seed of random to the current calendar time
	RandGenerator.seed(time(nullptr));
	
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
	m_camera->transform.position.z = -5.0f;

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

	// Create the water texture object
	TShared<ETexture> waterTexture = TMakeShared<ETexture>();
	// Add the texture to the mesh if exists
	if (!waterTexture->LoadTexture("Water texture", "Textures/T_Water.png")) {
		EDebug::Log("Graphics engine water texture did not load.", LT_ERROR);
	}

	// Create the wood texture object
	TShared<ETexture> woodTexture = TMakeShared<ETexture>();
	// Add the texture to the mesh if exists
	if (!woodTexture->LoadTexture("Wood texture", "Textures/T_Wood.png")) {
		EDebug::Log("Graphics engine wood texture did not load.", LT_ERROR);
	}

	// Create the grass texture object
	TShared<ETexture> grassTexture = TMakeShared<ETexture>();
	// Add the texture to the mesh if exists
	if (!grassTexture->LoadTexture("Grass texture", "Textures/T_Grass.png")) {
		EDebug::Log("Graphics engine grass texture did not load.", LT_ERROR);
	}

	// Create the space texture object
	TShared<ETexture> spaceTexture = TMakeShared<ETexture>();
	// Add the texture to the mesh if exists
	if (!spaceTexture->LoadTexture("Space texture", "Textures/T_Space.png")) {
		EDebug::Log("Graphics engine space texture did not load.", LT_ERROR);
	}

	// Create the red texture object
	TShared<ETexture> redTexture = TMakeShared<ETexture>();
	// Add the texture to the mesh if exists
	if (!redTexture->LoadTexture("Red texture", "Textures/T_Red.png")) {
		EDebug::Log("Graphics engine red texture did not load.", LT_ERROR);
	}

	// Log the success of the graphics engine initialisation
	EDebug::Log("Successfully initialised Graphics Engine.", LT_SUCCESS);

	// DEBUG
	TShared<EModel> modelSpike = TMakeShared<EModel>();
	modelSpike->MakeSpike(blackPlasticTexture);
	m_modelStack.push_back(std::move(modelSpike));

	TShared<EModel> modelCube = TMakeShared<EModel>();
	modelCube->MakeCube(coinTexture);
	modelCube->GetTransform().position.x = 4.0f;
	m_modelStack.push_back(std::move(modelCube));

	TShared<EModel> modelPlane = TMakeShared<EModel>();
	modelPlane->MakePlane(waterTexture);
	modelPlane->GetTransform().position.y = -4.0f;
	modelPlane->GetTransform().position.x = 2.0f;
	m_modelStack.push_back(std::move(modelPlane));

	// Create spike model objects with random textures
	for (int numberOfSpikes = 0; numberOfSpikes < SPIKE_NUM; numberOfSpikes++) {
		// Get random texture
		TWeak<ETexture> randomTexture;
		int randomTextureIndex = GetRandomIntRange(0, 7);
		switch (randomTextureIndex) {
		case 0:
			randomTexture = defaultTexture;
			break;
		case 1:
			randomTexture = blackPlasticTexture;
			break;
		case 2:
			randomTexture = coinTexture;
			break;
		case 3:
			randomTexture = waterTexture;
			break;
		case 4:
			randomTexture = woodTexture;
			break;
		case 5:
			randomTexture = grassTexture;
			break;
		case 6:
			randomTexture = spaceTexture;
			break;
		case 7:
			randomTexture = redTexture;
			break;
		}

		// Create object, assign texture, add to stack
		TShared<EModel> randomModelSpike = TMakeShared<EModel>();
		randomModelSpike->MakeSpike(randomTexture.lock());
		randomModelSpike->GetTransform().position.z = 10.0f;
		m_modelStack.push_back(std::move(randomModelSpike));
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

	// Activate shader
	m_shader->Activate();

	// Set the world transformations based on the camera
	m_shader->SetWorldTransform(m_camera);

	// Render custom graphics
	for (const auto& model : m_modelStack) {
		model->Render(m_shader);
	}

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}

void EGraphicsEngine::Update(float deltaTime)
{	
	// DEBUG 
	// Rotate spike model
	const auto spikeModel = GetModel(0).lock();
	spikeModel->GetTransform().rotation.x += 100.0f * deltaTime;
	spikeModel->GetTransform().rotation.y += 50.0f * deltaTime;

	// Move cube models
	// Stores the current move direction and multiplies the move speed by it...
	// ...each time the cube reaches the top or bottom of the movement
	static float cubeSpeed = 2.0f;

	// Check if cube outside range
	float movementRangeMax = 2.5f;
	const auto cubeModel = GetModel(1).lock();
	if (cubeModel->GetTransform().position.y > movementRangeMax ||
		cubeModel->GetTransform().position.y < -movementRangeMax) {
		// Flip the move speed so the cube moves in the opposite direction
		cubeSpeed *= -1.0f;
	}
	cubeModel->GetTransform().position.y += cubeSpeed * deltaTime;

	// Randomly move one of the other spike models
	glm::vec3 translation(
		GetRandomFloatRange(-1.0f, 1.0f),
		GetRandomFloatRange(-1.0f, 1.0f),
		GetRandomFloatRange(-1.0f, 1.0f)
	);
	int firstSpikeIndex = 3;
	const auto randOtherSpikeModel = GetModel(GetRandomIntRange(firstSpikeIndex, firstSpikeIndex + SPIKE_NUM - 1)).lock();
	randOtherSpikeModel->GetTransform().rotation.x += 60.0f * deltaTime;
	randOtherSpikeModel->GetTransform().rotation.y += 30.0f * deltaTime;
	randOtherSpikeModel->TranslateLocal(translation, glm::vec3(100.0f * deltaTime));
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

float EGraphicsEngine::GetRandomFloatRange(float min, float max) const
{
	std::uniform_real_distribution<float> RandNum(min, max);

	return RandNum(RandGenerator);
}

float EGraphicsEngine::GetRandomIntRange(int min, int max) const
{
	std::uniform_int_distribution<int> RandNum(min, max);

	return RandNum(RandGenerator);
}
