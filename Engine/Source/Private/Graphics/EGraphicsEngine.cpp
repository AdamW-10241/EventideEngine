#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"

// External Libs
#include <GLEW/glew.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include <string>

std::vector<ESVertexData> vertexData1;
std::vector<uint32_t> indexData1;

std::vector<ESVertexData> vertexData2;
std::vector<uint32_t> indexData2;
// Test mesh for debug
std::unique_ptr<EMesh> m_mesh1;
std::unique_ptr<EMesh> m_mesh2;

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
	m_mesh1 = std::make_unique<EMesh>();
	m_mesh2 = std::make_unique<EMesh>();

	// Square
	vertexData1.resize(4);
	// Vertex 1 TL
	vertexData1[0].m_position[0] = -0.25f;
	vertexData1[0].m_position[1] = 0.25f;
	vertexData1[0].m_position[2] = 0.0f;
	// Colour
	vertexData1[0].m_color[0] = 1.0f;
	vertexData1[0].m_color[1] = 1.0f;
	vertexData1[0].m_color[2] = 0.0f;
	// Vertex 2 BL
	vertexData1[1].m_position[0] = -0.25f;
	vertexData1[1].m_position[1] = -0.25f;
	vertexData1[1].m_position[2] = 0.0f;
	// Colour
	vertexData1[1].m_color[0] = 1.0f;
	vertexData1[1].m_color[1] = 1.0f;
	vertexData1[1].m_color[2] = 0.0f;
	// Vertex 3 BR
	vertexData1[2].m_position[0] = 0.25f;
	vertexData1[2].m_position[1] = -0.25f;
	vertexData1[2].m_position[2] = 0.0f;
	// Colour
	vertexData1[2].m_color[0] = 1.0f;
	vertexData1[2].m_color[1] = 1.0f;
	vertexData1[2].m_color[2] = 0.0f;
	// Vertex 4 TR
	vertexData1[3].m_position[0] = 0.25f;
	vertexData1[3].m_position[1] = 0.25f;
	vertexData1[3].m_position[2] = 0.0f;
	// Colour
	vertexData1[3].m_color[0] = 1.0f;
	vertexData1[3].m_color[1] = 1.0f;
	vertexData1[3].m_color[2] = 0.0f;

	// Triangle
	vertexData2.resize(3);
	// Vertex 1 L
	vertexData2[0].m_position[0] = -0.25f;
	vertexData2[0].m_position[1] = -0.25f;
	vertexData2[0].m_position[2] = 0.0f;
	// Colour
	vertexData2[0].m_color[0] = 1.0f;
	vertexData2[0].m_color[1] = 0.0f;
	vertexData2[0].m_color[2] = 0.0f;
	// Vertex 2 R
	vertexData2[1].m_position[0] = 0.25f;
	vertexData2[1].m_position[1] = -0.25f;
	vertexData2[1].m_position[2] = 0.0f;
	// Colour
	vertexData2[1].m_color[0] = 0.0f;
	vertexData2[1].m_color[1] = 1.0f;
	vertexData2[1].m_color[2] = 0.0f;
	// Vertex 3 T
	vertexData2[2].m_position[0] = 0.0f;
	vertexData2[2].m_position[1] = 0.25f;
	vertexData2[2].m_position[2] = 0.0f;
	// Colour
	vertexData2[2].m_color[0] = 0.0f;
	vertexData2[2].m_color[1] = 0.0f;
	vertexData2[2].m_color[2] = 1.0f;

	// Square
	indexData1.resize(6);
	indexData1[0] = 0; // Vertex 1 TL
	indexData1[1] = 1; // Vertex 2 BL
	indexData1[2] = 2; // Vertex 3 BR
	indexData1[3] = 0; // Vertex 4 TL
	indexData1[4] = 3; // Vertex 5 TR
	indexData1[5] = 2; // Vertex 6 BR

	// Triangle
	indexData2.resize(3);
	indexData2[0] = 0; // Vertex 7 L
	indexData2[1] = 1; // Vertex 8 R
	indexData2[2] = 2; // Vertex 9 T

	// Create the mesh and test if it failed
	if (!m_mesh1->CreateMesh(vertexData1, indexData1)) {
		EDebug::Log("Failed to create DEBUG mesh 1.");
	}

	// Create the mesh and test if it failed
	if (!m_mesh2->CreateMesh(vertexData2, indexData2)) {
		EDebug::Log("Failed to create DEBUG mesh 2.");
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

	static float rotationAmount = 0.05f;

	static float moveAmountX = 0.0001f;
	if (transform.position.x > 0.75 || transform.position.x < -0.75) {
		moveAmountX *= -1;
		rotationAmount *= -1;
	}

	static float moveAmountY = 0.00006f;
	if (transform.position.y > 0.75 || transform.position.y < -0.75) {
		moveAmountY *= -1;
		rotationAmount *= -1;
	}

	transform.position.x += moveAmountX;
	transform.position.y += moveAmountY;

	transform.rotation.z += rotationAmount;
	transform.scale = glm::vec3(0.75f);

	// Render custom graphics
	m_mesh1->Render(m_shader, transform);

	static ESTransform transform2;
	transform2.rotation.z += 0.01f;

	m_mesh2->Render(m_shader, transform2);

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
