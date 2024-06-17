#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include <string>

const std::vector<ESVertexData> vertexData = {
	 //  X,	    Y,    Z	        R,    G,    B		// Tex Coords
	{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f } }, // Vertex Data 1 - TL
	{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f } }, // Vertex Data 2 - TR
	{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f } }, // Vertex Data 3 - BL
	{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f } }  // Vertex Data 4 - BR
};

const std::vector<uint32_t> indexData = {
	0, 1, 2, // Tri 1
	1, 2, 3 // Tri 2
};

// Test DEBUG mesh
std::unique_ptr<EMesh> m_mesh;

bool EGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr) {
		EDebug::Log("SDL window was null.", LT_ERROR);
		EDebug::Log("Graphics Engine failed to initialise.", LT_ERROR);
		return false;
	}
	
	// Create an OpenGL context
	m_sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	// Test if the context failed
	if (m_sdlGLContext == nullptr) {
		EDebug::Log("SDL failed to create GL context: " + std::string(SDL_GetError()), LT_ERROR);
		EDebug::Log("Graphics Engine failed to initialise.", LT_ERROR);
		return false;
	}

	// Make the current context active for the SDL window
	// Test if it failed
	if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {
		EDebug::Log("SDL failed to make GL context current: " + std::string(SDL_GetError()), LT_ERROR);
		EDebug::Log("Graphics Engine failed to initialise.", LT_ERROR);
		return false;
	}

	if (vsync) {
		// Try enable adaptive vsync and test if it failed
		if (SDL_GL_SetSwapInterval(-1) != 0) {
			// Try enable standard vsync and test if it failed
			if (SDL_GL_SetSwapInterval(1) != 0) {
				EDebug::Log(
					"Graphics Engine failed to initialise vsync: " + std::string(SDL_GetError()), 
					LT_WARNING);
				return false;
			}
		}
	}

	// Initialise glew
	GLenum glewResult = glewInit();

	// Test if glew failed
	if (glewResult != GLEW_OK) {
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResult));
		EDebug::Log("Graphics Engine failed to initialise glew: " + errorMsg);
		return false;
	}

	m_shader = std::make_shared<EShaderProgram>();

	// Attempt to init shader and test if failed
	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"
	)) {
		EDebug::Log("Graphics engine failed to initialise due to shader failure.");
		return false;
	}

	// Log the success of the graphics engine initialisation
	EDebug::Log("Successfully initialised Graphics Engine.", LT_SUCCESS);
	
	// Create DEBUG meshes
	m_mesh = std::make_unique<EMesh>();

	// Create the mesh and test if it failed
	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		EDebug::Log("Failed to create DEBUG mesh.");
	}

	// Create the texture object
	TShared<ETexture> defaultTexture = TMakeShared<ETexture>();

	// Add the texture to the mesh if exists
	if (defaultTexture->LoadTexture("Default Texure", "Textures/T_DefaultGrid.png")) {
		m_mesh->SetTexture(defaultTexture);
	}

	return true;
}

void EGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background color
	glClearColor(0.10f, 0.10f, 0.20f, 1.0f);

	// Clear the back buffer with a solid color
	glClear(GL_COLOR_BUFFER_BIT);

	static ESTransform transform;
	//transform.rotation.z += 0.01f;

	// Render custom graphics
	m_mesh->Render(m_shader, transform);

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
