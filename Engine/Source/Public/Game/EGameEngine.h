#pragma once

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "EngineTypes.h"
#include "EWindow.h"
#include "Listeners/EInput.h"
#include "Graphics/EModel.h"
#include "Graphics/ESMaterial.h"

class EObject;

class EGameEngine {
public:
	// Get or create a game engine if one does not exist
	static EGameEngine* GetGameEngine();

	// Destroy the game engine
	static void DestroyEngine();

	// Run the game
	bool Run();

	// Get the games points
	float& GetPoints() { return m_points; }

	// Return the delta time between frames
	double DeltaTime() const { return m_deltaTime; }

	// Return the delta time between frames as a float
	float DeltaTimeF() const { return static_cast<float>(m_deltaTime); }

	// Create an EObject type
	// Also pass args
	template<typename T, std::enable_if_t<std::is_base_of_v<EObject, T>, T>* = nullptr, typename ... Args>
	TWeak<T> CreateObject(Args&&... args) {
		// Create an object within the template class
		TShared<T> newObject = TMakeShared<T>(std::forward<Args>(args)...);

		// Add the object into the stack
		m_objectsToBeSpawned.push_back(newObject);

		return newObject;
	}

	// Mark an object for destroy
	// All game objects destroy functions will automatically run this
	void DestroyObject(const TShared<EObject>& object);

	// Get a random float value between 2 floats
	float GetRandomFloatRange(float min = 0.0f, float max = 1.0f) const;

	// Get a random int value between 2 ints
	int GetRandomIntRange(int min = 0, int max = 1) const;

	// Set the frame rate
	void SetFrameRate(unsigned int frameRate) { m_frameRate = frameRate; }

	// Reset the frame rate
	void ResetFrameRate() { SetFrameRate(m_defaultFrameRate); }

	// Import a model from a path
	TWeak<EModel> ImportModel(const EString& path);

	// Create a material for the engine
	TShared<ESMaterial> CreateMaterial();

	// Create a material for the engine
	TShared<ESMaterial> CreateMaterialB(float brightness);

	// Get a reference to the graphics engine
	TUnique<EGraphicsEngine>& GetGraphicsEngine() { return m_window->GetGraphicsEngine(); }

	// Find an object of type T from the object stack
	template<typename T, std::enable_if_t<std::is_base_of_v<EObject, T>, T>* = nullptr>
	TWeak<T> FindObjectOfType() {
		// Find an object within the object stack=
		for (const auto& eObjectRef : m_objectStack) {
			if (const auto& cast = std::dynamic_pointer_cast<T>(eObjectRef)) {
				// Match
				return cast;
			}
				
		}

		// Else no match, return empty
		return {};
	}

private:
	// Constructor and destructor are private to ensure only 1 game engine
	EGameEngine();
	~EGameEngine();

	// Initialise all required libraries for the game
	bool Init();

	// Runs after initialise of the engine
	void Start();

	// Runs the loop of the game
	void GameLoop();

	// Cleanup the game engine
	void Cleanup();

	// LOOP FUNCTIONS
	// Run game logic for the frame
	void Tick();

	// Process the input for each frame
	void ProcessInput();

	// Render the graphics for each frame
	void Render();

	// Runs at the start of each loop
	void PreLoop();

	// Runs at the end of each loop
	void PostLoop();

private:
	// Store the window for the game engine
	TShared<EWindow> m_window;

	// Store the input for the game engine
	TShared<EInput> m_input;

	// Last time ticked
	double m_lastTickTime;

	double m_deltaTime;

	// Store all EObjects in the game
	TArray<TShared<EObject>> m_objectStack;

	// Store all EObjects to be started next frame
	TArray<TShared<EObject>> m_objectsToBeSpawned;

	// Store all EObjects that have been marked for destroy
	TArray<TShared<EObject>> m_objectsPendingDestroy;

	// Frame rate
	unsigned int m_frameRate;

	unsigned int m_defaultFrameRate;

	// Store time taken to load so it can be subtracted
	// Otherwise massive delta on first frame
	float m_timeToLoad;

	// Store the games points
	float m_points;
};