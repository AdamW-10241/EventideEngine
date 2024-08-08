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
	m_cameraRotation = glm::vec3(0.0f);

	m_canZoom = false;
	m_inputMode = false;
	m_doubleCameraSpeed = false;

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
	m_graphicsEngine = TMakeShared<EGraphicsEngine>();

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
	// Hide the cursor and set relative mouse mode
	m_input->ShowCursor(false);

	m_input->OnKeyPressed->Bind([this, m_input](const SDL_Scancode& key) {
		// Quick exit button for debug
		if (key == SDL_SCANCODE_ESCAPE) {
			CloseWindow();
		}
		// Toggle cursor visibility
		if (key == SDL_SCANCODE_PERIOD) {
			m_input->ShowCursor(m_input->IsCursorHidden());

			m_inputMode = !m_input->IsCursorHidden();
		}
		// Double camera speed
		if (key == SDL_SCANCODE_LSHIFT) {
			m_doubleCameraSpeed = true;
		}

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
			m_cameraDirection.y += -1.0f;
		}
		// Move down
		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += 1.0f;
		}
	});

	m_input->OnKeyReleased->Bind([this](const SDL_Scancode& key) {
		// Double camera speed
		if (key == SDL_SCANCODE_LSHIFT) {
			m_doubleCameraSpeed = false;
		}
		
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
			m_cameraDirection.y += 1.0f;
		}
		// Move down
		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += -1.0f;
		}
	});

	// On mouse move rotate the camera
	m_input->OnMouseMoved->Bind([this](const float& x, const float& y,
		const float& xrel, const float& yrel) {
			m_cameraRotation.y = -xrel;
			m_cameraRotation.x = -yrel;
		});

	// On mouse scroll zoom the camera
	m_input->OnMouseScrolled->Bind([this](const float& delta) {
		if (m_canZoom) {
			if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
				camRef->Zoom(delta);
			}
		}
	});

	m_input->OnMousePressed->Bind([this](const EUi8& button) {
		if (button == SDL_BUTTON_RIGHT) {
			m_canZoom = true;
		}
	});

	m_input->OnMouseReleased->Bind([this](const EUi8& button) {
		if (button == SDL_BUTTON_RIGHT) {
			m_canZoom = false;
			if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
				camRef->ResetZoom();
			}
		}
	});
}

void EWindow::Render()
{
	// Render the graphics engine if exists
	if (m_graphicsEngine) {
		// Test if there is a camera
		if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
			if (!m_inputMode) {
				// Translate the camera based on input direction
				camRef->Translate(m_cameraDirection, glm::vec3(m_doubleCameraSpeed ? 2 : 1));
				// Rotate the camera based on input direction
				camRef->Rotate(m_cameraRotation, glm::abs(m_cameraRotation));
			}
		}

		m_graphicsEngine->Render(m_sdlWindow);
	}
}
