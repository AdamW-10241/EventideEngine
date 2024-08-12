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

enum EEBackgroundColor : EUi8 {
	BC_DEFAULT = 0U,
	BC_RED,
	BC_GREEN,
	BC_BLUE,
	BC_WHITE,
	BC_BLACK
};

struct ESBackgroundColorData {
	float m_color[3] = { 0.0f, 0.0f, 0.0f };
};

const std::vector<ESBackgroundColorData> backgroundColorDataV{
//    R	    G     B
	{ 0.1f, 0.1f, 0.2f }, // Default
	{ 0.5f, 0.0f, 0.0f }, // Red
	{ 0.0f, 0.5f, 0.0f }, // Green
	{ 0.0f, 0.0f, 0.5f }, // Blue
	{ 1.0f, 1.0f, 1.0f }, // White
	{ 0.0f, 0.0f, 0.0f }  // Black
};

class EGraphicsEngine {
public:
	EGraphicsEngine();
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
	TShared<EModel> ImportModel(const EString& path);

	// Create a material for the engine
	TShared<ESMaterial> CreateMaterial();

	// Create a material for the engine
	TShared<ESMaterial> CreateMaterialB(float brightness);

	// Get a weak reference to the shader
	TWeak<EShaderProgram> GetShader() { return m_shader; }

	// Set the background color based on the input EEBackgroundColor
	void SetBackgroundColor(EEBackgroundColor backgroundColor) { m_backgroundColor = backgroundColor; }

	// Adjust the texture depth to be used in the shader
	void AdjustTextureDepth(float delta);

	// Reset the texture depth to be used in the shader
	void ResetTextureDepth();

	// Get the models stack
	TArray<TWeak<EModel>>& GetModels() { return m_models; }

	// Get the lights stack
	TArray<TShared<ESLight>>& GetLights() { return m_lights; }

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
	TArray<TWeak<EModel>> m_models;

	// Store the background color
	EEBackgroundColor m_backgroundColor;

	// Store a default material
	TShared<ESMaterial> m_defaultMaterial;
};
