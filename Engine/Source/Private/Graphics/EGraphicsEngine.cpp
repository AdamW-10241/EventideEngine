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

std::vector<ESVertexData> vertexData3;
std::vector<uint32_t> indexData3;

std::vector<ESVertexData> vertexData4;
std::vector<uint32_t> indexData4;

std::vector<ESVertexData> vertexData5;
std::vector<uint32_t> indexData5;

std::unique_ptr<EMesh> m_mesh1;
std::unique_ptr<EMesh> m_mesh2;
std::unique_ptr<EMesh> m_mesh3;
std::unique_ptr<EMesh> m_mesh4;
std::unique_ptr<EMesh> m_mesh5;

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
	m_mesh3 = std::make_unique<EMesh>();
	m_mesh4 = std::make_unique<EMesh>();
	m_mesh5 = std::make_unique<EMesh>();

	// Square
	vertexData1.resize(4);
	// Vertex 1 TL
	vertexData1[0].m_position[0] = -0.25f;
	vertexData1[0].m_position[1] = 0.25f;
	vertexData1[0].m_position[2] = 0.0f;
	// Colour 1
	vertexData1[0].m_color[0] = 1.0f;
	vertexData1[0].m_color[1] = 1.0f;
	vertexData1[0].m_color[2] = 0.0f;
	// Vertex 2 BL
	vertexData1[1].m_position[0] = -0.25f;
	vertexData1[1].m_position[1] = -0.25f;
	vertexData1[1].m_position[2] = 0.0f;
	// Colour 2
	vertexData1[1].m_color[0] = 1.0f;
	vertexData1[1].m_color[1] = 1.0f;
	vertexData1[1].m_color[2] = 0.0f;
	// Vertex 3 BR
	vertexData1[2].m_position[0] = 0.25f;
	vertexData1[2].m_position[1] = -0.25f;
	vertexData1[2].m_position[2] = 0.0f;
	// Colour 3
	vertexData1[2].m_color[0] = 1.0f;
	vertexData1[2].m_color[1] = 1.0f;
	vertexData1[2].m_color[2] = 0.0f;
	// Vertex 4 TR
	vertexData1[3].m_position[0] = 0.25f;
	vertexData1[3].m_position[1] = 0.25f;
	vertexData1[3].m_position[2] = 0.0f;
	// Colour 4
	vertexData1[3].m_color[0] = 1.0f;
	vertexData1[3].m_color[1] = 1.0f;
	vertexData1[3].m_color[2] = 0.0f;

	// Triangle
	vertexData2.resize(3);
	// Vertex 1 L
	vertexData2[0].m_position[0] = -0.25f;
	vertexData2[0].m_position[1] = -0.25f;
	vertexData2[0].m_position[2] = 0.0f;
	// Colour 1
	vertexData2[0].m_color[0] = 1.0f;
	vertexData2[0].m_color[1] = 0.0f;
	vertexData2[0].m_color[2] = 0.0f;
	// Vertex 2 R
	vertexData2[1].m_position[0] = 0.25f;
	vertexData2[1].m_position[1] = -0.25f;
	vertexData2[1].m_position[2] = 0.0f;
	// Colour 2
	vertexData2[1].m_color[0] = 0.0f;
	vertexData2[1].m_color[1] = 1.0f;
	vertexData2[1].m_color[2] = 0.0f;
	// Vertex 3 T
	vertexData2[2].m_position[0] = 0.0f;
	vertexData2[2].m_position[1] = 0.25f;
	vertexData2[2].m_position[2] = 0.0f;
	// Colour 3
	vertexData2[2].m_color[0] = 0.0f;
	vertexData2[2].m_color[1] = 0.0f;
	vertexData2[2].m_color[2] = 1.0f;

	// Circle
	vertexData3.resize(13);
	// Vertex 1
	vertexData3[0].m_position[0] = 0.0f;
	vertexData3[0].m_position[1] = 0.0f;
	vertexData3[0].m_position[2] = 0.0f;
	// Colour 1
	vertexData3[0].m_color[0] = 0.53f;
	vertexData3[0].m_color[1] = 0.06f;
	vertexData3[0].m_color[2] = 0.46f;
	// Vertex 2
	vertexData3[1].m_position[0] = 0.1f;
	vertexData3[1].m_position[1] = 0.4f;
	vertexData3[1].m_position[2] = 0.0f;
	// Colour 2
	vertexData3[1].m_color[0] = 0.66f;
	vertexData3[1].m_color[1] = 0.20f;
	vertexData3[1].m_color[2] = 0.33f;
	// Vertex 3
	vertexData3[2].m_position[0] = 0.3f;
	vertexData3[2].m_position[1] = 0.3f;
	vertexData3[2].m_position[2] = 0.0f;
	// Colour 3
	vertexData3[2].m_color[0] = 0.80f;
	vertexData3[2].m_color[1] = 0.40f;
	vertexData3[2].m_color[2] = 0.40f;
	// Vertex 4
	vertexData3[3].m_position[0] = 0.4f;
	vertexData3[3].m_position[1] = 0.1f;
	vertexData3[3].m_position[2] = 0.0f;
	// Colour 4
	vertexData3[3].m_color[0] = 0.93f;
	vertexData3[3].m_color[1] = 0.60f;
	vertexData3[3].m_color[2] = 0.26f;
	// Vertex 5
	vertexData3[4].m_position[0] = 0.4f;
	vertexData3[4].m_position[1] = -0.1f;
	vertexData3[4].m_position[2] = 0.0f;
	// Colour 5
	vertexData3[4].m_color[0] = 0.93f;
	vertexData3[4].m_color[1] = 0.86f;
	vertexData3[4].m_color[2] = 0.0f;
	// Vertex 6
	vertexData3[5].m_position[0] = 0.3f;
	vertexData3[5].m_position[1] = -0.3f;
	vertexData3[5].m_position[2] = 0.0f;
	// Colour 6
	vertexData3[5].m_color[0] = 0.60f;
	vertexData3[5].m_color[1] = 0.86f;
	vertexData3[5].m_color[2] = 0.33f;
	// Vertex 7
	vertexData3[6].m_position[0] = 0.1f;
	vertexData3[6].m_position[1] = -0.4f;
	vertexData3[6].m_position[2] = 0.0f;
	// Colour 7
	vertexData3[6].m_color[0] = 0.26f;
	vertexData3[6].m_color[1] = 0.86f;
	vertexData3[6].m_color[2] = 0.53f;
	// Vertex 8
	vertexData3[7].m_position[0] = -0.1f;
	vertexData3[7].m_position[1] = -0.4f;
	vertexData3[7].m_position[2] = 0.0f;
	// Colour 8
	vertexData3[7].m_color[0] = 0.13f;
	vertexData3[7].m_color[1] = 0.80f;
	vertexData3[7].m_color[2] = 0.73f;
	// Vertex 9
	vertexData3[8].m_position[0] = -0.3f;
	vertexData3[8].m_position[1] = -0.3f;
	vertexData3[8].m_position[2] = 0.0f;
	// Colour 9
	vertexData3[8].m_color[0] = 0.0f;
	vertexData3[8].m_color[1] = 0.73f;
	vertexData3[8].m_color[2] = 0.80f;
	// Vertex 10
	vertexData3[9].m_position[0] = -0.4f;
	vertexData3[9].m_position[1] = -0.1f;
	vertexData3[9].m_position[2] = 0.0f;
	// Colour 10
	vertexData3[9].m_color[0] = 0.0f;
	vertexData3[9].m_color[1] = 0.60f;
	vertexData3[9].m_color[2] = 0.80f;
	// Vertex 11
	vertexData3[10].m_position[0] = -0.4f;
	vertexData3[10].m_position[1] = 0.1f;
	vertexData3[10].m_position[2] = 0.0f;
	// Colour 11
	vertexData3[10].m_color[0] = 0.20f;
	vertexData3[10].m_color[1] = 0.40f;
	vertexData3[10].m_color[2] = 0.73f;
	// Vertex 12
	vertexData3[11].m_position[0] = -0.3f;
	vertexData3[11].m_position[1] = 0.3f;
	vertexData3[11].m_position[2] = 0.0f;
	// Colour 12
	vertexData3[11].m_color[0] = 0.40f;
	vertexData3[11].m_color[1] = 0.20f;
	vertexData3[11].m_color[2] = 0.60f;
	// Vertex 13
	vertexData3[12].m_position[0] = -0.1f;
	vertexData3[12].m_position[1] = 0.4f;
	vertexData3[12].m_position[2] = 0.0f;
	// Colour 13
	vertexData3[12].m_color[0] = 0.40f;
	vertexData3[12].m_color[1] = 0.20f;
	vertexData3[12].m_color[2] = 0.53f;

	// Star
	vertexData4.resize(6);
	// Vertex 1 L
	vertexData4[0].m_position[0] = -0.25f;
	vertexData4[0].m_position[1] = -0.25f;
	vertexData4[0].m_position[2] = 0.0f;
	// Colour 1
	vertexData4[0].m_color[0] = 0.6f;
	vertexData4[0].m_color[1] = 1.0f;
	vertexData4[0].m_color[2] = 0.2f;
	// Vertex 2 R
	vertexData4[1].m_position[0] = 0.25f;
	vertexData4[1].m_position[1] = -0.25f;
	vertexData4[1].m_position[2] = 0.0f;
	// Colour 2
	vertexData4[1].m_color[0] = 0.6f;
	vertexData4[1].m_color[1] = 1.0f;
	vertexData4[1].m_color[2] = 0.2f;
	// Vertex 3 T
	vertexData4[2].m_position[0] = 0.0f;
	vertexData4[2].m_position[1] = 0.1f;
	vertexData4[2].m_position[2] = 0.0f;
	// Colour 3
	vertexData4[2].m_color[0] = 0.6f;
	vertexData4[2].m_color[1] = 1.0f;
	vertexData4[2].m_color[2] = 0.2f;
	// Vertex 4 L
	vertexData4[3].m_position[0] = -0.25f;
	vertexData4[3].m_position[1] = 0.0f;
	vertexData4[3].m_position[2] = 0.0f;
	// Colour 4
	vertexData4[3].m_color[0] = 0.6f;
	vertexData4[3].m_color[1] = 1.0f;
	vertexData4[3].m_color[2] = 0.2f;
	// Vertex 5 R
	vertexData4[4].m_position[0] = 0.25f;
	vertexData4[4].m_position[1] = 0.0f;
	vertexData4[4].m_position[2] = 0.0f;
	// Colour 5
	vertexData4[4].m_color[0] = 0.6f;
	vertexData4[4].m_color[1] = 1.0f;
	vertexData4[4].m_color[2] = 0.2f;
	// Vertex 6 T
	vertexData4[5].m_position[0] = 0.0f;
	vertexData4[5].m_position[1] = -0.35f;
	vertexData4[5].m_position[2] = 0.0f;
	// Colour 6
	vertexData4[5].m_color[0] = 0.6f;
	vertexData4[5].m_color[1] = 1.0f;
	vertexData4[5].m_color[2] = 0.2f;

	// Arrow
	vertexData5.resize(7);
	// Vertex 1 TL
	vertexData5[0].m_position[0] = -0.25f;
	vertexData5[0].m_position[1] = 0.25f;
	vertexData5[0].m_position[2] = 0.0f;
	// Colour 1
	vertexData5[0].m_color[0] = 0.0f;
	vertexData5[0].m_color[1] = 0.6f;
	vertexData5[0].m_color[2] = 0.8f;
	// Vertex 2 BL
	vertexData5[1].m_position[0] = -0.25f;
	vertexData5[1].m_position[1] = -0.25f;
	vertexData5[1].m_position[2] = 0.0f;
	// Colour 2
	vertexData5[1].m_color[0] = 0.0f;
	vertexData5[1].m_color[1] = 0.6f;
	vertexData5[1].m_color[2] = 0.8f;
	// Vertex 3 BR
	vertexData5[2].m_position[0] = 0.25f;
	vertexData5[2].m_position[1] = -0.25f;
	vertexData5[2].m_position[2] = 0.0f;
	// Colour 3
	vertexData5[2].m_color[0] = 0.0f;
	vertexData5[2].m_color[1] = 0.6f;
	vertexData5[2].m_color[2] = 0.8f;
	// Vertex 4 TR
	vertexData5[3].m_position[0] = 0.25f;
	vertexData5[3].m_position[1] = 0.25f;
	vertexData5[3].m_position[2] = 0.0f;
	// Colour 4
	vertexData5[3].m_color[0] = 0.0f;
	vertexData5[3].m_color[1] = 0.6f;
	vertexData5[3].m_color[2] = 0.8f;
	// Vertex 5 L
	vertexData5[4].m_position[0] = 0.25f;
	vertexData5[4].m_position[1] = 0.5f;
	vertexData5[4].m_position[2] = 0.0f;
	// Colour 5
	vertexData5[4].m_color[0] = 0.0f;
	vertexData5[4].m_color[1] = 0.6f;
	vertexData5[4].m_color[2] = 0.8f;
	// Vertex 6 R
	vertexData5[5].m_position[0] = 0.25f;
	vertexData5[5].m_position[1] = -0.5f;
	vertexData5[5].m_position[2] = 0.0f;
	// Colour 6
	vertexData5[5].m_color[0] = 0.0f;
	vertexData5[5].m_color[1] = 0.6f;
	vertexData5[5].m_color[2] = 0.8f;
	// Vertex 7 T
	vertexData5[6].m_position[0] = 0.75f;
	vertexData5[6].m_position[1] = 0.0f;
	vertexData5[6].m_position[2] = 0.0f;
	// Colour 7
	vertexData5[6].m_color[0] = 0.0f;
	vertexData5[6].m_color[1] = 0.6f;
	vertexData5[6].m_color[2] = 0.8f;

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

	// Circle
	indexData3.resize(36);
	indexData3[0] = 0;
	indexData3[1] = 2;
	indexData3[2] = 3;
	indexData3[3] = 0;
	indexData3[4] = 3;
	indexData3[5] = 4;
	indexData3[6] = 0;
	indexData3[7] = 4;
	indexData3[8] = 5;
	indexData3[9] = 0;
	indexData3[10] = 5;
	indexData3[11] = 6;
	indexData3[12] = 0;
	indexData3[13] = 6;
	indexData3[14] = 7;
	indexData3[15] = 0;
	indexData3[16] = 7;
	indexData3[17] = 8;
	indexData3[18] = 0;
	indexData3[19] = 8;
	indexData3[20] = 9;
	indexData3[21] = 0;
	indexData3[22] = 9;
	indexData3[23] = 10;
	indexData3[24] = 0;
	indexData3[25] = 10;
	indexData3[26] = 11;
	indexData3[27] = 0;
	indexData3[28] = 11;
	indexData3[29] = 12;
	indexData3[30] = 0;
	indexData3[31] = 12;
	indexData3[32] = 1;
	indexData3[33] = 0;
	indexData3[34] = 1;
	indexData3[35] = 2;

	// Star
	indexData4.resize(6);
	indexData4[0] = 0;
	indexData4[1] = 1;
	indexData4[2] = 2;
	indexData4[3] = 3;
	indexData4[4] = 4;
	indexData4[5] = 5;

	// Arrow
	indexData5.resize(9);
	indexData5[0] = 0; // Vertex 1 TL
	indexData5[1] = 1; // Vertex 2 BL
	indexData5[2] = 2; // Vertex 3 BR
	indexData5[3] = 0; // Vertex 4 TL
	indexData5[4] = 3; // Vertex 5 TR
	indexData5[5] = 2; // Vertex 6 BR
	indexData5[6] = 4; // Vertex 7 T
	indexData5[7] = 5; // Vertex 8 B
	indexData5[8] = 6; // Vertex 9 R

	// Create the square mesh and test if it failed
	if (!m_mesh1->CreateMesh(vertexData1, indexData1)) {
		EDebug::Log("Failed to create DEBUG mesh 1.");
	}

	// Create the triangle mesh and test if it failed
	if (!m_mesh2->CreateMesh(vertexData2, indexData2)) {
		EDebug::Log("Failed to create DEBUG mesh 2.");
	}

	// Create the circle mesh and test if it failed
	if (!m_mesh3->CreateMesh(vertexData3, indexData3)) {
		EDebug::Log("Failed to create DEBUG mesh 3.");
	}

	// Create the star mesh and test if it failed
	if (!m_mesh4->CreateMesh(vertexData4, indexData4)) {
		EDebug::Log("Failed to create DEBUG mesh 4.");
	}

	// Create the arrow mesh and test if it failed
	if (!m_mesh5->CreateMesh(vertexData5, indexData5)) {
		EDebug::Log("Failed to create DEBUG mesh 5.");
	}

	return true;
}

void EGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background color
	glClearColor(0.10f, 0.10f, 0.20f, 1.0f);

	// Clear the back buffer with a solid color
	glClear(GL_COLOR_BUFFER_BIT);

	// Square
	static ESTransform transformSquare;
	transformSquare.rotation.z = 45.0f;
	transformSquare.position.x = 0.5f;
	transformSquare.position.y = 0.5f;

	// Triangle
	static ESTransform transformTriangle;
	transformTriangle.position.x = -0.5f;
	transformTriangle.position.y = -0.5f;

	static float triangleScaleAmount = 0.0003f;
	static float triangleScale = 1.0f;

	if (triangleScale > 2.0f || triangleScale < 0.5f) {
		triangleScaleAmount *= -1;
	}
	triangleScale += triangleScaleAmount;

	transformTriangle.scale = glm::vec3(triangleScale);

	// Circle
	static ESTransform transformCircle;

	static float circleRotationSpeed = 0.01f;
	static float circleMoveAmountX = 0.0001f;
	static float circleMoveAmountY = 0.00006f;

	RotateAndBounce(transformCircle, circleRotationSpeed, circleMoveAmountX, circleMoveAmountY);
	transformCircle.scale = glm::vec3(0.75f);

	// Star
	static ESTransform transformStar;

	static float starRotationSpeed = 0.04f;
	static float starMoveAmountX = 0.0002f;
	static float starMoveAmountY = -0.00015f;

	RotateAndBounce(transformStar, starRotationSpeed, starMoveAmountX, starMoveAmountY);

	transformStar.rotation.x += 0.02f;
	transformStar.rotation.y += 0.01f;

	static float starScaleAmount = 0.0001f;
	static float starScale = 1.0f;

	if (starScale > 1.0f || starScale < 0.5f) {
		starScaleAmount *= -1;
	}
	starScale += starScaleAmount;

	transformStar.scale = glm::vec3(starScale);

	// Arrow
	static ESTransform transformArrow;

	transformArrow.rotation.x += 0.01f;
	transformArrow.rotation.y += 0.02f;
	transformArrow.rotation.z += 0.03f;

	transformArrow.scale = glm::vec3(0.5f);

	// Render custom graphics
	m_mesh1->Render(m_shader, transformSquare);
	m_mesh5->Render(m_shader, transformArrow);
	m_mesh2->Render(m_shader, transformTriangle);

	m_mesh3->Render(m_shader, transformCircle);
	m_mesh4->Render(m_shader, transformStar);

	// Swap the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}

void EGraphicsEngine::RotateAndBounce(static ESTransform& transform, static float& rotationSpeed, static float& moveAmountX, static float& moveAmountY)
{
	// Change X move and rotation direction for X wall bounces
	if (transform.position.x > 0.75 || transform.position.x < -0.75) {
		moveAmountX *= -1;
		rotationSpeed *= -1;
	}

	// Change Y move and rotation direction for Y wall bounces
	if (transform.position.y > 0.75 || transform.position.y < -0.75) {
		moveAmountY *= -1;
		rotationSpeed *= -1;
	}

	// Move transform
	transform.position.x += moveAmountX;
	transform.position.y += moveAmountY;

	// Rotate transform
	transform.rotation.z += rotationSpeed;
}