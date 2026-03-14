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
#include "Game/GameObjects/CustomObjects/Coin.h"
#include "Graphics/EText.h"
#include "Graphics/ESprite.h"

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
		EDebug::Log(("Game Engine failed to initialise."), LT_ERROR);
		return false;
	}

	Start();

	GameLoop();

	return true;
}

void EGameEngine::AddPoints(int points, bool createText)
{
	// Add points
	m_points += points;

	if (!createText) return;
	
	// Create HUD text
	glm::vec2 spawnPos;
	if (auto window = EGameEngine::GetGameEngine()->GetWindow().lock()) {
		spawnPos = { 100.0f, window->GetWindowSize().y - (window->GetWindowSize().y / 3.0f) }; // WindowSize y = height
	}

	ESAddSpriteConfig config("Fonts/Press_Start_2P/PressStart2P-Regular.ttf", spawnPos, 0);
	config.SetIsText(true);
	config.SetRenderColor({ 1.0f, 1.0f, 0.0f, 1.0f });

	if (const auto& screenObj = EGameEngine::GetGameEngine()->CreateObject<EScreenObject>().lock()) {
		screenObj->SetLifeTime(1.5f);
		auto spriteText = screenObj->AddSprite(config);
		if (auto spriteRef = spriteText.lock()) {
			if (auto textRef = TCast<EText>(spriteRef)) {
				textRef->SetFontSize(14);
				textRef->SetText("+" + toEString(points) + " Points!");
			}
		}

		BIND_EVENT_SELF(screenObj, OnTicked, [](const TShared<EObject>& obj, float deltaTime) {
			if (const auto& screen = TCast<EScreenObject>(obj)) {
				if (const auto& sprite = screen->GetSprite(0).lock()) {
					sprite->GetTransform().position.y += 60.0f * deltaTime;
					sprite->SetRenderColorAlpha(obj->GetLifeTimeRatio());
				}
			}
			});
	}
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

	m_defaultFrameRate = 240;
	m_frameRate = m_defaultFrameRate;

	m_timeToLoad = 0.0f;
	m_gameState = EGameState::NONE;

	m_points = 0;

	// Set random seed
	RandGenerator.seed((unsigned int)time(0));
	
	EDebug::Log("Game Engine created.");
}

EGameEngine::~EGameEngine()
{
	Cleanup();
	
	EDebug::Log("Game Engine destroyed.");
}

bool EGameEngine::Init()
{
	// Initialise the components of SDL that we need
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		EDebug::Log("Failed to init SDL: " + EString(SDL_GetError()), LT_ERROR);
		return false;
	}

	if (TTF_Init() != 0) {
		EDebug::Log("Failed to init SDL TTF: " + EString(TTF_GetError()), LT_ERROR);
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
		//3440, 1440 })) {
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
		if (auto weapon = EGameEngine::GetGameEngine()->CreateObject<Weapon>(player, true, 1.0f, 2000.0f, 0.1f, false).lock()) {
			player->AddWeapon(weapon);
		}

		// Add crosshair
		if (const auto crosshair = CreateObject<EScreenObject>(0).lock()) {
			ESTransform2D transform;
			transform.position = m_window->GetWindowCenter();
			const auto& sprite = crosshair->AddSprite(ESAddSpriteConfig{ "Sprites/Crosshairs/crosshair009.png", transform, 0 });
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

	// Spawn Coin
	auto coin = CreateObject<Coin>().lock();
	coin->Destroy();

	// Create Points HUD text
	{
		glm::vec2 spawnPos{ 100.0f, GetWindow().lock()->GetWindowSize().y - 100.0f };
		ESAddSpriteConfig config("Fonts/Press_Start_2P/PressStart2P-Regular.ttf", spawnPos, 0);
		config.SetIsText(true);
		config.SetRenderColor({ 1.0f, 1.0f, 0.0f, 1.0f });

		if (const auto& pointsObj = EGameEngine::GetGameEngine()->CreateObject<EScreenObject>().lock()) {
			auto spriteText = pointsObj->AddSprite(config);
			if (auto textRef = TCast<EText>(spriteText.lock())) {
				textRef->SetFontSize(16);
			}

			BIND_EVENT_SELF(pointsObj, OnTicked, [](const TShared<EObject>& obj, float deltaTime) {
				if (const auto& points = TCast<EScreenObject>(obj)) {
					if (const auto& text = TCast<EText>(points->GetSprite(0).lock())) {
						EString newText = "Points - " + toEString(EGameEngine::GetGameEngine()->GetPoints());
						text->SetText(newText);
					}
				}
				});
		}
	}

	// Create Points HUD text
	{
		glm::vec2 spawnPos{ 100.0f, 100.0f };
		ESAddSpriteConfig config("Fonts/Press_Start_2P/PressStart2P-Regular.ttf", spawnPos, 0);
		config.SetIsText(true);
		config.SetRenderColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		if (const auto& fpsObj = EGameEngine::GetGameEngine()->CreateObject<EScreenObject>().lock()) {
			auto spriteText = fpsObj->AddSprite(config);
			if (auto textRef = TCast<EText>(spriteText.lock())) {
				textRef->SetFontSize(16);
			}

			BIND_EVENT_SELF(fpsObj, OnTicked, [](const TShared<EObject>& obj, float deltaTime) {
				if (const auto& points = TCast<EScreenObject>(obj)) {
					if (const auto& text = TCast<EText>(points->GetSprite(0).lock())) {
						EString newText = "FPS - " + toEString((int)floor(1 / EGameEngine::GetGameEngine()->DeltaTime()));
						text->SetText(newText);
					}
				}
				});
		}
	}

	// DEBUG GUI Button
	//if (auto buttonRef = CreateObject<GUIButton>(1).lock()) {
	//	auto spriteBase = buttonRef->AddSprite(ESAddSpriteConfig{ "Sprites/Button/QuitButton.png", m_window->GetWindowCenter(), 0 });
	//	if (auto spriteRef = spriteBase.lock()) {
	//		spriteRef->GetTransform().SetScaleMultiCentered(1.0f);
	//	}

	//	ESAddSpriteConfig config("Fonts/Press_Start_2P/PressStart2P-Regular.ttf", m_window->GetWindowCenter(), 0);
	//	config.SetIsText(true);
	//	config.SetRenderColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	//	auto spriteText = buttonRef->AddSprite(config);
	//	if (auto spriteRef = spriteText.lock()) {
	//		if (auto textRef = TCast<EText>(spriteRef)) {
	//			textRef->SetFontSize(20);
	//			textRef->SetText("Test!");
	//		}
	//		spriteRef->GetTransform().SetScaleMultiCentered(1.0f);
	//	}

	//	buttonRef->SetSpritePressedColor(0, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
	//	buttonRef->SetSpritePressedColor(1, glm::vec4(0.4f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f ));
	//	BIND_EVENT_SELF(buttonRef, OnPressed, [](const TShared<GUIButton>& btn) {
	//		btn->SetSpritesRenderScales(0.9f);
	//	});
	//	BIND_EVENT_SELF(buttonRef, OnReleased, [](const TShared<GUIButton>& btn) {
	//		btn->SetSpritesRenderScales(1.0f);
	//	});
	//}

	// Get the time to load
	m_timeToLoad = static_cast<double>(SDL_GetTicks64());

	// Set game state
	m_gameState = EGameState::GAME;
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

		// Only delay if we're running faster than target frame rate
		double targetDeltaMilli = 1000.0 / m_frameRate;
		if (deltaMilli < targetDeltaMilli) {
			SDL_Delay((EUi32)(targetDeltaMilli - deltaMilli));
		}

		// We must detect input, react with logic and then render based on logic
		PreLoop();
		ProcessInput();

		// Process all engine tick functions
		if (m_gameState == EGameState::GAME)
			Tick();

		Render();
		PostLoop();
	}
}

void EGameEngine::Cleanup()
{
	// Destroy all objects first
	m_objectStack.clear();
	m_objectsToBeSpawned.clear();
	m_objectsPendingDestroy.clear();

	// Then destroy window (which holds graphics/sprites)
	m_input = nullptr;
	m_window = nullptr;

	// Now safe to quit SDL subsystems
	TTF_Quit();
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
	// Player is an addon to camera currently - this movement should be made part of player?
	if (m_window)
		m_window->MoveCamera();

	// Tick all objects first
	for (const auto& obj : m_objectStack) {
		obj->Tick(DeltaTimeF());
	}

	// Build grid for testing collisions after ticking for updated positions
	RebuildSpatialGrid();

	// Test collisions
	TestCollisions();

	// PostTick all objects after collision check
	for (const auto& obj : m_objectStack) {
		obj->PostTick(DeltaTimeF());
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
		if (const auto& eLightObjectRef = TCast<ELightObject>(eObjectRef)) {
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

void EGameEngine::RebuildSpatialGrid()
{
	m_spatialGrid.clear();

	for (const auto& obj : m_objectStack) {
		if (const auto& wo = TCast<EWorldObject>(obj)) {
			if (wo->HasCollisions()) {
				// Register in every cell the object overlaps
				for (const auto& cell : GetOccupiedCells(wo)) {
					m_spatialGrid[cell].push_back(wo);
				}
			}
		}
	}
}

void EGameEngine::TestCollisions()
{
	// Store tested pairs
	std::set<std::pair<void*, void*>> testedPairs;

	// Collision detection using spatial grid
	for (const auto& obj : m_objectStack) {
		const auto& wo = TCast<EWorldObject>(obj);
		if (!wo || !wo->HasCollisions()) continue;

		EGridCell center = GetCell(wo->GetTransform().position);

		// Iterate cell and neighbours
		for (int dx = -1; dx <= 1; dx++) {
			for (int dz = -1; dz <= 1; dz++) {
				auto it = m_spatialGrid.find({ center.x + dx, center.y + dz });
				if (it == m_spatialGrid.end()) continue;

				for (const auto& other : it->second) {
					if (wo == other) continue;

					// Check if already tested pair
					void* pA = wo.get();
					void* pB = other.get();
					if (pA > pB) std::swap(pA, pB);

					if (testedPairs.count({ pA, pB })) continue;
					testedPairs.insert({ pA, pB });

					// Test collisions
					wo->TestCollision(other);
					other->TestCollision(wo);
				}
			}
		}
	}
}

TArray<EGridCell> EGameEngine::GetOccupiedCells(const TShared<EWorldObject>& wo) const
{
	TArray<EGridCell> cells;
	
	// Iterate collisions
	auto collisions = wo->GetCollisions();
	for (auto col : collisions) {
		if (const auto& colRef = col.lock()) {
			// Compute min/max cell extents
			EGridCell minCell = GetCell(col.lock()->box.GetMin());
			EGridCell maxCell = GetCell(col.lock()->box.GetMax());

			// Add overlapping cells
			for (int x = minCell.x; x <= maxCell.x; x++) {
				for (int z = minCell.y; z <= maxCell.y; z++) {
					cells.push_back({ x, z });
				}
			}
		}
	}

	return cells;
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