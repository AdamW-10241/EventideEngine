#include "Game/EGameEngine.h"
#include "Game/GameObjects/EObject.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EShaderProgram.h"

// External Livsd
#include <random>

// Initialise a random generator
std::default_random_engine RandGenerator;

// DEBUG
#include "Game/GameObjects/EPteraObject.h"
#include "Game/GameObjects/ELightObject.h"

EGameEngine* EGameEngine::GetGameEngine()
{
	static EGameEngine* instance = new EGameEngine();
	
	return instance;
}

void EGameEngine::DestroyEngine()
{
	delete GetGameEngine();
}

bool EGameEngine::Run()
{
	if (!Init()) {
		EDebug::Log(("Game engine failed to initialise"), LT_ERROR);
		return false;
	}

	Start();

	GameLoop();

	return true;
}

void EGameEngine::DestroyObject(const TShared<EObject>& object)
{
	m_objectsPendingDestroy.push_back(object);
}

TWeak<EModel> EGameEngine::ImportModel(const EString& path)
{
	return m_window->GetGraphicsEngine().lock()->ImportModel(path);
}

TShared<ESMaterial> EGameEngine::CreateMaterial()
{
	return m_window->GetGraphicsEngine().lock()->CreateMaterial();
}

TShared<ESMaterial> EGameEngine::CreateMaterialB(float brightness)
{
	return m_window->GetGraphicsEngine().lock()->CreateMaterialB(brightness);
}

EGameEngine::EGameEngine()
{
	m_lastTickTime = 0.0f;
	m_deltaTime = 0.0f;

	m_defaultFrameRate = 240;
	m_frameRate = m_defaultFrameRate;

	m_timeToLoad = 0.0f;

	// Set random seed
	RandGenerator.seed(time(0));
	
	EDebug::Log("Game engine created");
}

EGameEngine::~EGameEngine()
{
	Cleanup();
	
	EDebug::Log("Game engine destroyed");
}

bool EGameEngine::Init()
{
	// Initialise the components of SDL that we need
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		EDebug::Log("Failed to init SDL: " + EString(SDL_GetError()), LT_ERROR);
		return false;
	}

	// Tell SDL that we will be rendering in OpenGL version 460 or 4.60
	// 4 is the major version
	// .60 is the minor version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Make sure compativbility mode is run for an OpenGL version not in SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	// Set the bit depth for each colour
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	// Creating the window object
	m_window = TMakeShared<EWindow>();

	// Creating an SDL window
	if (!m_window->CreateWindow({ "Game Window",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		720, 720 })) {
		return false;
	}

	// Create the input class and assign the window
	m_input = TMakeShared<EInput>();
	m_input->InitInput(m_window);
	
	return true;
}

void EGameEngine::Start()
{
	// Register the window inputs
	m_window->RegisterInput(m_input);

	// Spawn Ptera Objects
	for (int i = 0; i <= 10; i++)
		CreateObject<EPteraObject>();

	// Spawn Grid
	TWeak<EModelObject> gridModel = CreateObject<EModelObject>();
	gridModel.lock()->AddModel("Models/Grid/grid.fbx");
	// Grid base color texture
	TShared<ETexture> gridBaseTex = TMakeShared<ETexture>();
	gridBaseTex->LoadTexture("Grid Base Colour", "Models/Remains/textures/Ground_baseColor.png");
	TShared<ETexture> gridNormal = TMakeShared<ETexture>();
	gridNormal->LoadTexture("Grid Base Colour", "Models/Remains/textures/Ground_normal.png");
	// Grid material
	TShared<ESMaterial> gridMat = CreateMaterial();
	gridMat->m_baseColourMap = gridBaseTex;
	gridMat->m_normalMap = gridNormal;
	// Setting the grid material to the material slot
	gridModel.lock()->GetModel().lock()->SetMaterialBySlot(0, gridMat);
	// Adjust placement and size
	gridModel.lock()->GetTransform().scale = glm::vec3(0.02f, 1.0f, 0.02f);
	gridModel.lock()->GetTransform().position.y = -1.6f;

	// Spawn grass at a random position on the floor grid based on the mesh
	for (int i = 0; i < 10; i++) {
		// Spawn Grass
		TWeak<EModelObject> grassModel = CreateObject<EModelObject>();
		grassModel.lock()->AddModel("Models/Grass/Grass_green.fbx");
		// Grid base color texture
		TShared<ETexture> grassBaseTex = TMakeShared<ETexture>();
		grassBaseTex->LoadTexture("Grass Base Colour", "Models/Grass/textures/Grass_green.png");
		TShared<ETexture> grassNormal = TMakeShared<ETexture>();
		grassNormal->LoadTexture("Grass Normals", "Models/Grass/textures/Normal_grass.png");
		// Grid material
		TShared<ESMaterial> grassMat = CreateMaterial();
		grassMat->m_baseColourMap = grassBaseTex;
		grassMat->m_normalMap = grassNormal;
		// Setting the grid material to the material slot
		grassModel.lock()->GetModel().lock()->SetMaterialBySlot(0, grassMat);
		// Adjust size
		grassModel.lock()->GetTransform().scale = glm::vec3(0.01f);

		// Get a random position on the model from its mesh and set a new object to that position
		glm::vec3 randomPositionOnModel = gridModel.lock()->GetModel().lock()->GetMeshStack().at(0)->GetRandomVertexPosition();
		grassModel.lock()->GetTransform().position = randomPositionOnModel;
		// Adjust to height of grid
		grassModel.lock()->GetTransform().position.y -= 1.6f;
	}

	// Get the time to load
	m_timeToLoad = static_cast<double>(SDL_GetTicks64());
}

void EGameEngine::GameLoop()
{
	// Keep the game open as long as the window is open
	while (!m_window->IsPendingClose()) {
		// Create delta time
		// Set the current tick time
		// SDL_GetTicks64() gets the time since the engine started in milliseconds
		double curTickTime = static_cast<double>(SDL_GetTicks64()) - m_timeToLoad;
		// Convert the tick time into delta milli
		// How much time has passed since the last frame
		double deltaMilli = curTickTime - m_lastTickTime;
		// Convert delta milli to seconds
		m_deltaTime = deltaMilli / 1000;
		// Update the last tick time to the current tick time for the loop
		m_lastTickTime = curTickTime;

		// Caps the frame rate
		int frameDuration = 1000 / m_frameRate;

		if ((double)frameDuration > deltaMilli) {
			frameDuration = int(deltaMilli);
		}

		// If the frame rate is greater than m_frameRate, delay the frame
		SDL_Delay((EUi32)frameDuration);

		// The order of these functions is important
		// We must detect input, react with logic and then render based on logic
		PreLoop();

		// Process all engine input functions
		ProcessInput();

		// Process all engine tick functions
		Tick();

		// Process all engine render functions
		Render();
		
		PostLoop();
	}
}

void EGameEngine::Cleanup()
{
	m_input = nullptr;
	m_window = nullptr;

	SDL_Quit();
}

void EGameEngine::Tick()
{
	// Run through all EObjects in the game and run their ticks
	for (const auto& eObjectRef : m_objectStack) {
		eObjectRef->Tick(DeltaTimeF());
		eObjectRef->PostTick(DeltaTimeF());
	}

	// Randomly change brightness if flag set (LEFT CTRL)
	if (m_window->m_randomlyChangeBrightness) {
		float randBrightness = GetRandomFloatRange(0.5f, 1.5f);
		m_window->GetGraphicsEngine().lock()->GetShader().lock()->SetBrightness(randBrightness);
	}

	// Create random light after time
	static float timeUntilNextLight = 0.0f;
	timeUntilNextLight -= m_deltaTime;

	if (timeUntilNextLight <= 0.0f) {
		TWeak<ELightObject> light = CreateObject<ELightObject>();

		// Get random values
		glm::vec3 randColour = glm::vec3(
			GetRandomFloatRange(0.0f, 1.0f),
			GetRandomFloatRange(0.0f, 1.0f), 
			GetRandomFloatRange(0.0f, 1.0f)
		);
		glm::vec3 randPosition = glm::vec3(
			GetRandomFloatRange(-10.0f, 10.0f),
			GetRandomFloatRange(-10.0f, 10.0f),
			GetRandomFloatRange(-10.0f, 10.0f)
		);
		float randIntensity = GetRandomFloatRange(0.5, 2.0f);
		float randLifeTime = GetRandomFloatRange(1.0f, 5.0f);
		 
		light.lock()->AddPointLight(randColour, randIntensity, randPosition);
		light.lock()->SetLifeTime(randLifeTime);

		timeUntilNextLight = GetRandomFloatRange(0.5f, 4.0f);
	}
}

void EGameEngine::ProcessInput()
{
	if (!m_input)
		return;

	// Handle inputs
	m_input->UpdateInputs();
}

void EGameEngine::Render()
{
	if (!m_window)
		return;

	// Render the window
	m_window->Render();
}

void EGameEngine::PreLoop()
{
	// Running through all objects to be spawned
	// Run their start logic and add to game object stack
	for (auto& eObjectRef : m_objectsToBeSpawned) {
		eObjectRef->Start();
		m_objectStack.push_back(std::move(eObjectRef));
	}

	m_objectsToBeSpawned.clear();
}

void EGameEngine::PostLoop()
{
	// Get stacks stack
	TArray<TShared<EModel>>& eModelStack = m_window->GetGraphicsEngine().lock()->GetModels();
	TArray<TShared<ESLight>>& eLightStack = m_window->GetGraphicsEngine().lock()->GetLights();
	
	// Loop through all objects pending destroy
	// Remove their references from the object stack
	for (const auto& eObjectRef : m_objectsPendingDestroy) {
		auto it = std::find(m_objectStack.begin(), m_objectStack.end(), eObjectRef);

		if (it == m_objectStack.end())
			continue;
		
		// Cleanup models from the model stack (otherwise they stay forever)
		if (const auto& eModelObjectRef = std::dynamic_pointer_cast<EModelObject>(eObjectRef)) {
			// Get the model
			const auto& eModel = eModelObjectRef->GetModel();
			
			// Find the model objects model
			auto modelIt = std::find(eModelStack.begin(), eModelStack.end(), eModel.lock());
			
			// If it was found then erase it
			if (modelIt != eModelStack.end())
				eModelStack.erase(modelIt);
		}

		// Cleanup lights from the lights stack (otherwise they stay forever)
		if (const auto& eLightObjectRef = std::dynamic_pointer_cast<ELightObject>(eObjectRef)) {
			// Get the model
			const auto& eLight = eLightObjectRef->GetPointLight();

			// Find the light objects light
			auto lightIt = std::find(eLightStack.begin(), eLightStack.end(), eLight.lock());

			// If it was found then erase it
			if (lightIt != eLightStack.end())
				eLightStack.erase(lightIt);
		}

		m_objectStack.erase(it);
	}

	// Make sure the clear the pending destroy array so no references remain
	m_objectsPendingDestroy.clear();
}

float EGameEngine::GetRandomFloatRange(float min, float max) const
{
	std::uniform_real_distribution<float> RandNum(min, max);

	return RandNum(RandGenerator);
}

float EGameEngine::GetRandomIntRange(int min, int max) const
{
	std::uniform_int_distribution<int> RandNum(min, max);

	return RandNum(RandGenerator);
}