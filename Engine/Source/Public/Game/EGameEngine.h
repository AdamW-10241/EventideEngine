#pragma once

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "EngineTypes.h"
#include "EWindow.h"
#include "Listeners/EInput.h"

class EObject;

class EGameEngine {
public:
	// Get or create a game engine if one does not exist
	static EGameEngine* GetGameEngine();

	// Destroy the game engine
	static void DestroyEngine();

	// Run the game
	bool Run();

	// Return the delta time between frames
	double DeltaTime() const { return m_deltaTime; }

	// Return the delta time between frames as a float
	float DeltaTimeF() const { return static_cast<float>(m_deltaTime); }

	// Create an EObject type
	template<typename T, std::enable_if_t<std::is_base_of_v<EObject, T>, T>* = nullptr>
	TWeak<T> CreateObject() {
		// Create an object within the template class
		TShared<T> newObject = TMakeShared<T>();

		// Add the object into the stack
		m_objectsToBeSpawned.push_back(newObject);

		return newObject;
	}

	// Mark an object for destroy
	// All game objects destroy functions will automatically run this
	void DestroyObject(const TShared<EObject>& object);

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
};