// Engine Libs
#include "EngineTypes.h"
#include "Game/EGameEngine.h"

int main(int argc, char* argv[]) {
	int result = 0;

	// Initialise the engine
	// Test if Init fails
	if (!EGameEngine::GetGameEngine()->Run()) {
		result = -1;
	}

	// Clean up the engine
	EGameEngine::DestroyEngine();
	return result;
}