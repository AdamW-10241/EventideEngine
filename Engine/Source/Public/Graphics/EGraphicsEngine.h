#pragma once
#include <iostream>

typedef void* SDL_GLContext;
struct SDL_Window;
class EMesh;
class EShaderProgram;
struct ESTransform;

class EGraphicsEngine {
public:
	EGraphicsEngine() = default;
	~EGraphicsEngine() = default;

	// Initialise the graphics engine
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// Render the graphics engine
	void Render(SDL_Window* sdlWindow);

private:
	// Rotate and bounce mesh around space
	void RotateAndBounce(static ESTransform& transform, static float& rotationSpeed, static float& moveAmountX, static float& moveAmountY);

private:
	// Storing memory location for OpenGL context
	SDL_GLContext m_sdlGLContext;

	// Store the shader for the engine
	std::shared_ptr<EShaderProgram> m_shader;
};
