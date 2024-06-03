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

	vertexData.resize(7);
	// Vertex 1 TL
	vertexData[0].m_position[0] = 0.0f;
	vertexData[0].m_position[1] = 0.5f;
	vertexData[0].m_position[2] = 0.0f;
	// Vertex 2 BL
	vertexData[1].m_position[0] = 0.0f;
	vertexData[1].m_position[1] = -0.5f;
	vertexData[1].m_position[2] = 0.0f;
	// Vertex 3 BR
	vertexData[2].m_position[0] = 0.5f;
	vertexData[2].m_position[1] = -0.5f;
	vertexData[2].m_position[2] = 0.0f;
	// Vertex 4 TR
	vertexData[3].m_position[0] = 0.5f;
	vertexData[3].m_position[1] = 0.5f;
	vertexData[3].m_position[2] = 0.0f;

	// Vertex 5 L
	vertexData[4].m_position[0] = -0.75f;
	vertexData[4].m_position[1] = 0.0f;
	vertexData[4].m_position[2] = 0.0f;
	// Vertex 6 R
	vertexData[5].m_position[0] = -0.25f;
	vertexData[5].m_position[1] = 0.0f;
	vertexData[5].m_position[2] = 0.0f;
	// Vertex 7 T
	vertexData[6].m_position[0] = -0.5f;
	vertexData[6].m_position[1] = sqrtf(3.0f) / 2.0f;
	vertexData[6].m_position[2] = 0.0f;

	indexData.resize(9);
	// Square
	indexData[0] = 0; // Vertex 1 TL
	indexData[1] = 1; // Vertex 2 BL
	indexData[2] = 2; // Vertex 3 BR
	indexData[3] = 0; // Vertex 4 TL
	indexData[4] = 3; // Vertex 5 TR
	indexData[5] = 2; // Vertex 6 BR
	// Triangle
	indexData[6] = 4; // Vertex 7 L
	indexData[7] = 5; // Vertex 8 R
	indexData[8] = 6; // Vertex 9 T

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
	glClearColor(0.15f, 0.15f, 0.25f, 1.0f);

	// Clear the back buffer with a solid color
	glClear(GL_COLOR_BUFFER_BIT);

	// Render custom graphics
	m_mesh->Render();

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
