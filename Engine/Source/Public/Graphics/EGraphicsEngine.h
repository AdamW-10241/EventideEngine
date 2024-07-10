#pragma once
#include "EngineTypes.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class EMesh;
class EShaderProgram;
struct ESCamera;

class EGraphicsEngine {
public:
	EGraphicsEngine() = default;
	~EGraphicsEngine() = default;

	// Initialise the graphics engine
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// Render the graphics engine
	void Render(SDL_Window* sdlWindow);

	// Update the graphics engine
	void Update(float deltaTime);

	// Return a weak version of the camera
	TWeak<ESCamera> GetCamera() { return m_camera; }

private:
	// Storing memory location for OpenGL context
	SDL_GLContext m_sdlGLContext;

	// Store the shader for the engine
	TShared<EShaderProgram> m_shader;

	// Store the camera
	TShared<ESCamera> m_camera;
};
