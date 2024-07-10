#pragma once
#include "EngineTypes.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class EShaderProgram;
struct ESCamera;
class EModel;

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

const std::vector<ESBackgroundColorData> backgroundColorDataV {
//	  R	    G     B
	{ 0.1f, 0.1f, 0.2f }, // Default
	{ 0.5f, 0.0f, 0.0f }, // Red
	{ 0.0f, 0.5f, 0.0f }, // Green
	{ 0.0f, 0.0f, 0.5f }, // Blue
	{ 1.0f, 1.0f, 1.0f }, // White
	{ 0.0f, 0.0f, 0.0f }  // Black
};

class EGraphicsEngine {
public:
	EGraphicsEngine() { m_backgroundColor = BC_DEFAULT; }
	~EGraphicsEngine() = default;

	// Initialise the graphics engine
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// Render the graphics engine
	void Render(SDL_Window* sdlWindow);

	// Update the graphics engine
	void Update(float deltaTime);

	// Return a weak version of the camera
	TWeak<ESCamera> GetCamera() { return m_camera; }

	// Set the background color based on the input EEBackgroundColor
	void SetBackgroundColor(EEBackgroundColor backgroundColor) { m_backgroundColor = backgroundColor; }

	// Return a weak version of the camera
	TWeak<EModel> GetModel(int modelStackIndex) { return m_modelStack.at(modelStackIndex); }

private:
	// Storing memory location for OpenGL context
	SDL_GLContext m_sdlGLContext;

	// Store the shader for the engine
	TShared<EShaderProgram> m_shader;

	// Store the camera
	TShared<ESCamera> m_camera;

	// Store the background color
	EEBackgroundColor m_backgroundColor;

	// Store the models
	TArray<TShared<EModel>> m_modelStack;
};
