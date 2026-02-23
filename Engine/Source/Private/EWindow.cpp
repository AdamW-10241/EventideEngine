#include "EWindow.h"
#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"
#include "Listeners/EInput.h"
#include "Graphics/ESCamera.h"
#include "Game/EGameEngine.h"
#include "Graphics/EShaderProgram.h"

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
	m_randomlyChangeBrightness = false;

	m_canAdjustTextureDepth = false;

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
	m_graphicsEngine = TMakeUnique<EGraphicsEngine>();

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
		// Set background color to default
		if (key == SDL_SCANCODE_1) {
			if (m_graphicsEngine)
				m_graphicsEngine->SetBackgroundColor(BC_DEFAULT);
		}
		// Set background color to red
		if (key == SDL_SCANCODE_2) {
			if (m_graphicsEngine)
				m_graphicsEngine->SetBackgroundColor(BC_RED);
		}
		// Set background color to green
		if (key == SDL_SCANCODE_3) {
			if (m_graphicsEngine)
				m_graphicsEngine->SetBackgroundColor(BC_GREEN);
		}
		// Set background color to blue
		if (key == SDL_SCANCODE_4) {
			if (m_graphicsEngine)
				m_graphicsEngine->SetBackgroundColor(BC_BLUE);
		}
		// Set background color to white
		if (key == SDL_SCANCODE_5) {
			if (m_graphicsEngine)
				m_graphicsEngine->SetBackgroundColor(BC_WHITE);
		}
		// Set background color to black
		if (key == SDL_SCANCODE_6) {
			if (m_graphicsEngine)
				m_graphicsEngine->SetBackgroundColor(BC_BLACK);
		}
		
		// Quick exit button for debug
		if (key == SDL_SCANCODE_ESCAPE) {
			CloseWindow();
		}
		// Toggle cursor visibility
		if (key == SDL_SCANCODE_PERIOD) {
			m_input->ShowCursor(m_input->IsCursorHidden());

			m_inputMode = !m_input->IsCursorHidden();
		}
		// Toggle camera vertical status
		if (key == SDL_SCANCODE_COMMA) {
			// Get and toggle the status
			bool& status = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()->GetVerticalMovementStatus();
			status = !status;

			// Debug logs
			if (status) {
				EDebug::Log("Can move vertically.");
			}
			else {
				EDebug::Log("Can no longer move vertically.");
			}
		}
		// Double camera speed
		if (key == SDL_SCANCODE_LSHIFT) {
			m_doubleCameraSpeed = true;
		}
		// Set frame rate to 10
		if (key == SDL_SCANCODE_TAB) {
			EGameEngine::GetGameEngine()->SetFrameRate(10);
		}
		// Set flag to randomly change brightness
		if (key == SDL_SCANCODE_LCTRL) {
			m_randomlyChangeBrightness = true;
		}

		// Rotate camera up
		if (key == SDL_SCANCODE_UP) {
			m_cameraRotation.x += -1.0f;
		}
		// Rotate camera down
		if (key == SDL_SCANCODE_DOWN) {
			m_cameraRotation.x += 1.0f;
		}
		// Rotate camera left
		if (key == SDL_SCANCODE_RIGHT) {
			m_cameraRotation.y += -1.0f;
		}
		// Rotate camera right
		if (key == SDL_SCANCODE_LEFT) {
			m_cameraRotation.y += 1.0f;
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
		// Reset frame rate
		if (key == SDL_SCANCODE_TAB) {
			EGameEngine::GetGameEngine()->ResetFrameRate();
		}
		// Set flag to randomly change brightness
		if (key == SDL_SCANCODE_LCTRL) {
			m_randomlyChangeBrightness = false;
			// Reset to default 1.0f
			EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetShader().lock()->SetBrightness(1.0f);
		}
		
		// Rotate camera up
		if (key == SDL_SCANCODE_UP) {
			m_cameraRotation.x += 1.0f;
		}
		// Rotate camera down
		if (key == SDL_SCANCODE_DOWN) {
			m_cameraRotation.x += -1.0f;
		}
		// Rotate camera left
		if (key == SDL_SCANCODE_RIGHT) {
			m_cameraRotation.y += 1.0f;
		}
		// Rotate camera right
		if (key == SDL_SCANCODE_LEFT) {
			m_cameraRotation.y += -1.0f;
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
			m_cameraRotation.y += -xrel;
			m_cameraRotation.x += yrel;
	});

	// On mouse scroll
	m_input->OnMouseScrolled->Bind([this](const float& delta) {
		if (!m_inputMode) {
			// Zoom camera
			if (m_canZoom) {
				if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
					camRef->Zoom(delta);
				}
			}
			// Adjust the texture depth
			if (m_canAdjustTextureDepth) {
				if (m_graphicsEngine)
					m_graphicsEngine->AdjustTextureDepth(delta * 0.05f);
			}
		}
	});

	m_input->OnMousePressed->Bind([this](const EUi8& button) {
		if (!m_inputMode) {
			if (button == SDL_BUTTON_RIGHT) {
				m_canZoom = true;
			}
			if (button == SDL_BUTTON_LEFT) {
				m_canAdjustTextureDepth = true;
			}
		}
	});

	m_input->OnMouseReleased->Bind([this](const EUi8& button) {
		if (!m_inputMode) {
			if (button == SDL_BUTTON_RIGHT) {
				m_canZoom = false;
				if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
					camRef->ResetZoom();
				}
			}
			if (button == SDL_BUTTON_LEFT) {
				m_canAdjustTextureDepth = false;
				if (m_graphicsEngine)
					m_graphicsEngine->ResetTextureDepth();
			}
		}
	});
}

void EWindow::MoveCamera()
{
	// Test if there is a camera
	if (const auto& camRef = GetGraphicsEngine()->GetCamera().lock()) {
		if (!m_inputMode) {
			// Translate the camera based on input direction
			camRef->Translate(m_cameraDirection, glm::vec3(m_doubleCameraSpeed ? 2.0f : 1.0f));
			// Rotate the camera based on input direction
			camRef->Rotate(m_cameraRotation);
			// Reset rotations
			m_cameraRotation.x = 0.0f;
			m_cameraRotation.y = 0.0f;
		}
	}
}

void EWindow::Render()
{
	// Render the graphics engine if exists
	if (m_graphicsEngine)
		m_graphicsEngine->Render(m_sdlWindow);
}
