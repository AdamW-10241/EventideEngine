#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"
#include "Graphics/EMesh.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

std::vector<ESVertexData> vertexData;
std::vector<uint32_t> indexData;
// Test mesh for debug
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

	// Log the success of the graphics engine initialisation
	EDebug::Log("Successfully initialised Graphics Engine.", LT_SUCCESS);
	
	// Create a DEBUG mesh
	m_mesh = std::make_unique<EMesh>();

	vertexData.resize(3);
	// Vertex 1
	vertexData[0].m_position[0] = 0.0f;
	vertexData[0].m_position[1] = 0.5f;
	vertexData[0].m_position[2] = 0.0f;
	// Vertex 2
	vertexData[1].m_position[0] = -0.5f;
	vertexData[1].m_position[1] = -0.5f;
	vertexData[1].m_position[2] = 0.0f;
	// Vertex 3
	vertexData[2].m_position[0] = 0.5f;
	vertexData[2].m_position[1] = -0.5f;
	vertexData[2].m_position[2] = 0.0f;

	indexData.resize(3);
	indexData[0] = 0; // Vertex 1
	indexData[1] = 1; // Vertex 2
	indexData[2] = 2; // Vertex 3

	// Create the mesh and test if it failed
	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		EDebug::Log("Failed to create DEBUG mesh.");
	}

	return true;
}

void EGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Wireframe shader
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Set a background color
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid color
	glClear(GL_COLOR_BUFFER_BIT);

	// Render custom graphics
	m_mesh->Render();

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
