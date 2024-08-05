#pragma once
#include "EngineTypes.h"
#include "Graphics/ESMaterial.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class EShaderProgram;
struct ESCamera;
class EModel;
struct ESLight;
struct ESPointLight;
struct ESDirLight;

class EGraphicsEngine {
public:
	EGraphicsEngine() = default;
	~EGraphicsEngine() = default;

	// Initialise the graphics engine
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// Render the graphics engine
	void Render(SDL_Window* sdlWindow);

	// Return a weak version of the camera
	TWeak<ESCamera> GetCamera() { return m_camera; }

	// Create a point light and return a weak pointer
	TWeak<ESPointLight> CreatePointLight();

	// Create a directional light and return a weak pointer
	TWeak<ESDirLight> CreateDirLight();

	// Import a model and return a weak pointer
	TWeak<EModel> ImportModel(const EString& path);

	// Create a material for the engine
	TShared<ESMaterial> CreateMaterial();

private:
	// Storing memory location for OpenGL context
	SDL_GLContext m_sdlGLContext;

	// Store the shader for the engine
	TShared<EShaderProgram> m_shader;

	// Store the camera
	TShared<ESCamera> m_camera;

	// Stores all the lights in the engine
	TArray<TShared<ESLight>> m_lights;

	// Stores all the models in the engine
	TArray<TShared<EModel>> m_models;
};
