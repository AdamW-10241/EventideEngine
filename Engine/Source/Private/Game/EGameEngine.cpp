#include "Game/EGameEngine.h"
#include "Game/GameObjects/EObject.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EShaderProgram.h"

// External Libs
#include <random>

// Initialise a random generator
std::default_random_engine RandGenerator;

// DEBUG
#include "Game/GameObjects/CustomObjects/Player.h"
#include "Game/GameObjects/CustomObjects/Floor.h"
#include "Game/GameObjects/CustomObjects/Grass.h"
#include "Game/GameObjects/CustomObjects/Weapon.h"
#include "Game/GameObjects/CustomObjects/Bullet.h"
#include "Game/GameObjects/CustomObjects/Enemy.h"
#include "Game/GameObjects/CustomObjects/Skybox.h"
#include "Game/GameObjects/CustomObjects/Wall.h"
#include "Game/GameObjects/CustomObjects/InvisibleWalls.h"
#include "Game/GameObjects/CustomObjects/GUIButton.h"

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
	return m_window->GetGraphicsEngine()->ImportModel(path);
}

TShared<ESMaterial> EGameEngine::CreateMaterial()
{
	return m_window->GetGraphicsEngine()->CreateMaterial();
}

TShared<ESMaterial> EGameEngine::CreateMaterialB(float brightness)
{
	return m_window->GetGraphicsEngine()->CreateMaterialB(brightness);
}

EGameEngine::EGameEngine()
{
	m_lastTickTime = 0.0f;
	m_deltaTime = 0.0f;

	m_defaultFrameRate = 120;
	m_frameRate = m_defaultFrameRate;

	m_timeToLoad = 0.0f;

	m_points = 0;

	// Set random seed
	RandGenerator.seed((unsigned int)time(0));
	
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
	// Loading log
	EDebug::Log("\nLoading...\n");
	
	// Register the window inputs
	m_window->RegisterInput(m_input);

	// Spawn Skybox
	CreateObject<Skybox>();

	// Spawn Floor
	CreateObject<Floor>();

	// Spawn Invisble Walls (edge of map)
	CreateObject<InvisibleWalls>();

	// Spawn Walls
	for (EUi32 i = 0; i < 15; i++) {
		CreateObject<Wall>();
	}

	// Spawn Player
	if (const auto& player = CreateObject<Player>().lock()) {
		// Set default camera position
		player->SetDefaultCamPosition({ 0.0f, 20.0f, 0.0f });

		// Add weapon
		if (auto weapon = EGameEngine::GetGameEngine()->CreateObject<Weapon>(player, true, 1.0f, 500.0f, 0.2f, false).lock()) {
			player->AddWeapon(weapon);
		}

		// Add crosshair
		if (const auto crosshair = CreateObject<EScreenObject>(0).lock()) {
			ESTransform2D transform;
			transform.position = m_window->GetWindowCenter();
			const auto& sprite = crosshair->AddSprite("Sprites/Crosshairs/crosshair009.png", transform, 0);
			if (const auto& spriteRef = sprite.lock()) {
				spriteRef->GetTransform().scale *= 0.4f;
				spriteRef->GetTransform().CenterOnPosition();
			}
			player->AddCrosshair(crosshair);
		}

		// Spawn Enemies
		for (EUi32 i = 0; i < 8; i++) {
			// Spawn enemy
			Enemy::SpawnEnemy(player);
		}
	}

	// Spawn Grass
	for (EUi32 i = 0; i < 30; i++) {
		// Spawn grass
		CreateObject<Grass>();
	}

	// DEBUG GUI Button
	// CreateObject<GUIButton>(1);

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
	// Randomly change brightness if flag set (LEFT CTRL)
	if (m_window->m_randomlyChangeBrightness) {
		float randBrightness = GetRandomFloatRange(0.5f, 1.5f);
		m_window->GetGraphicsEngine()->GetShader().lock()->SetBrightness(randBrightness);
	}
	
	// Move Camera
	if (m_window)
		m_window->MoveCamera();

	// Run through all EObjects in the game and run their ticks
	for (const auto& eObjectRef : m_objectStack) {
		eObjectRef->Tick(DeltaTimeF());

		// Check if object is a world object, otherwise skip logic 
		if (const auto& woRef = std::dynamic_pointer_cast<EWorldObject>(eObjectRef)) {
			// Check if world object has collisions
			if (woRef->HasCollisions()) {
				// Loop through all objects to test against
				for (const auto& otherObj : m_objectStack) {
					// Test if the other object is also a world object
					if (const auto& otherWoRef = std::dynamic_pointer_cast<EWorldObject>(otherObj)) {
						// Skip colliding with self
						if (woRef == otherWoRef)
							continue;

						// Check if other ref has collisions
						if (!otherWoRef->HasCollisions())
							continue;

						// If all is good then test if their collisions are overlapping
						woRef->TestCollision(otherWoRef);
					}
				}
			}
		}

		eObjectRef->PostTick(DeltaTimeF());
	}
}

void EGameEngine::ProcessInput()
{
	if (!m_input)
		return;

	// Update input events
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
		eObjectRef->RegisterInputs(m_input);
		m_objectStack.push_back(std::move(eObjectRef));
	}

	m_objectsToBeSpawned.clear();
}

void EGameEngine::PostLoop()
{
	// Get stacks stack
	TArray<TShared<ESLight>>& eLightStack = m_window->GetGraphicsEngine()->GetLights();
	
	// Loop through all objects pending destroy
	// Remove their references from the object stack
	for (const auto& eObjectRef : m_objectsPendingDestroy) {
		auto it = std::find(m_objectStack.begin(), m_objectStack.end(), eObjectRef);

		if (it == m_objectStack.end())
			continue;

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

int EGameEngine::GetRandomIntRange(int min, int max) const
{
	std::uniform_int_distribution<int> RandNum(min, max);

	return RandNum(RandGenerator);
}