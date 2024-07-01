#include "EWindow.h"
#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"
#include "Listeners/EInput.h"
#include "Graphics/ESCamera.h"

// External Libs
#include <SDL/SDL.h>

EWindow::EWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;
	m_cameraDirection = glm::vec3(0.0f);

	EDebug::Log("Window created.");
}

EWindow::~EWindow()
{
	// If the SDL window exists, destroy it
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	EDebug::Log("Window destroyed.");
}

bool EWindow::CreateWindow(const ESWindowParams& params)
{
	// Enabling OpenGL in our SDL window
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	// Assigning the parameters to the member for the window
	m_params = params;

	// Add vsync flag if selected
	if (m_params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	// Add fullscreen flag if selected
	if (m_params.fullscreen)
		// Fullscreen borderless
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		// Windowed mode
		windowFlags += SDL_WINDOW_SHOWN;

	// Create the SDL window
	m_sdlWindow = SDL_CreateWindow(
		m_params.title.c_str(),
		m_params.x,
		m_params.y,
		m_params.w,
		m_params.h,
		windowFlags
	);

	// Check if SDL window was created
	if (!m_sdlWindow) {
		EDebug::Log(
			"SDL failed to create window: " + EString(SDL_GetError()),
			LT_ERROR);
		CloseWindow();
		return false;
	}

	// Create the graphics engine objects
	m_graphicsEngine = std::make_unique<EGraphicsEngine>();

	// Initialise the graphics engine and test if it failed
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		EDebug::Log("Window failed to initialise Graphics Engine.", LT_ERROR);
		m_graphicsEngine = nullptr;
		return false;
	}

	return true;
}

void EWindow::RegisterInput(const TShared<EInput>& m_input)
{
	m_input->OnKeyPressed->Bind([this](const SDL_Scancode& key) {
		// Move forward
		if (key == SDL_SCANCODE_W) {
			m_cameraDirection.z += 1.0f;
		}
		// Move backward
		if (key == SDL_SCANCODE_S) {
			m_cameraDirection.z += -1.0f;
		}
		// Move left
		if (key == SDL_SCANCODE_A) {
			m_cameraDirection.x += -1.0f;
		}
		// Move right
		if (key == SDL_SCANCODE_D) {
			m_cameraDirection.x += 1.0f;
		}
		// Move up
		if (key == SDL_SCANCODE_Q) {
			m_cameraDirection.y += 1.0f;
		}
		// Move down
		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += -1.0f;
		}
	});

	m_input->OnKeyReleased->Bind([this](const SDL_Scancode& key) {
		// Move forward
		if (key == SDL_SCANCODE_W) {
			m_cameraDirection.z += -1.0f;
		}
		// Move backward
		if (key == SDL_SCANCODE_S) {
			m_cameraDirection.z += 1.0f;
		}
		// Move left
		if (key == SDL_SCANCODE_A) {
			m_cameraDirection.x += 1.0f;
		}
		// Move right
		if (key == SDL_SCANCODE_D) {
			m_cameraDirection.x += -1.0f;
		}
		// Move up
		if (key == SDL_SCANCODE_Q) {
			m_cameraDirection.y += -1.0f;
		}
		// Move down
		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += 1.0f;
		}
	});
}

void EWindow::Render()
{
	// Render the graphics engine if exists
	if (m_graphicsEngine) {
		// Test if there is a camera
		if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
			// If there is a cmaera, move it based on camera direction input
			camRef->transform.position += m_cameraDirection * 0.001f;
		}

		m_graphicsEngine->Render(m_sdlWindow);
	}
}
