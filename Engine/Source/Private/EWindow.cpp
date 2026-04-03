#include "EWindow.h"
#include "Game/EGameEngine.h"
#include "Graphics/EGraphicsEngine.h"
#include "Debug/EDebug.h"
#include "Listeners/EInput.h"
#include "Graphics/ESCamera.h"
#include "Graphics/EShaderProgram.h"

// External Libs
#include <SDL/SDL.h>
#include <GLEW/glew.h>

EWindow::EWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;
	m_cameraRotation = glm::vec3(0.0f);

	m_canZoom = false;
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
		EDebug::Log(LT_WARNING, "SDL failed to create window: " + EString(SDL_GetError()));
		CloseWindow();
		return false;
	}

	// Create the graphics engine objects
	m_graphicsEngine = TMakeUnique<EGraphicsEngine>();

	// Initialise the graphics engine and test if it failed
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		EDebug::Log(LT_WARNING, "Window failed to initialise Graphics Engine.");
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
		// Toggle pause
		if (key == SDL_SCANCODE_PERIOD) {
			m_input->ShowCursor(m_input->IsCursorHidden());
			if (m_input->IsCursorHidden()) {
				EGameEngine::GetGameEngine()->SetGameState(EEGameState::GAME);
			}
			else {
				EGameEngine::GetGameEngine()->SetGameState(EEGameState::PAUSED);
			}
		}
		// Toggle fullscreen
		if (key == SDL_SCANCODE_TAB) {
			auto bounds = GetDisplayBounds(); // Display Size
			glm::vec2 defaultSize = GetDefaultSize(); // Default Size
			glm::vec2 currentSize = GetCurrentSize(); // Current Size

			if (defaultSize.x != currentSize.x || defaultSize.y != currentSize.y) {
				SDL_SetWindowSize(m_sdlWindow, defaultSize.x, defaultSize.y);
				glm::vec2 position{ ((float)bounds.w) / 2.0f, ((float)bounds.h) / 2.0f };
				position -= (defaultSize / 2.0f);
				SDL_SetWindowPosition(m_sdlWindow, position.x, position.y);
				m_params.fullscreen = false;
			}
			else {
				SDL_SetWindowSize(m_sdlWindow, bounds.w, bounds.h);
				SDL_SetWindowPosition(m_sdlWindow, 0, 0);
				m_params.fullscreen = true;
			}
		}
		// Quick exit button for debug
		if (key == SDL_SCANCODE_ESCAPE) {
			CloseWindow();
		}
		if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;

		// Toggle camera vertical status
		if (key == SDL_SCANCODE_COMMA) {
			// Get and toggle the status
			bool& status = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetCamera().lock()->GetVerticalMovementStatus();
			status = !status;

			// Debug logs
			if (status) { EDebug::Log("Can move vertically."); }
			else { EDebug::Log("Can no longer move vertically."); }
		}
		// Double camera speed
		if (key == SDL_SCANCODE_LSHIFT) {
			m_doubleCameraSpeed = true;
		}
		// Set flag to randomly change brightness
		if (key == SDL_SCANCODE_LCTRL) {
			m_randomlyChangeBrightness = true;
		}
	});

	m_input->OnKeyReleased->Bind([this, m_input](const SDL_Scancode& key) {
		if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;

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
	});

	// On mouse move rotate the camera
	m_input->OnMouseMoved->Bind([this, m_input](const float& x, const float& y,
		const float& xrel, const float& yrel) {
			if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;

			m_cameraRotation.y += -xrel;
			m_cameraRotation.x += yrel;
	});

	// On mouse scroll
	m_input->OnMouseScrolled->Bind([this, m_input](const float& delta) {
		if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;

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
	});

	m_input->OnMousePressed->Bind([this, m_input](const EUi8& button) {
		// Toggle pause
		if (button == SDL_BUTTON_MIDDLE) {
			m_input->ShowCursor(m_input->IsCursorHidden());
			if (m_input->IsCursorHidden()) {
				EGameEngine::GetGameEngine()->SetGameState(EEGameState::GAME);
			}
			else {
				EGameEngine::GetGameEngine()->SetGameState(EEGameState::PAUSED);
			}
		}
		if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;
		
		if (button == SDL_BUTTON_RIGHT) {
			m_canZoom = true;
		}
		if (button == SDL_BUTTON_LEFT) {
			m_canAdjustTextureDepth = true;
		}
	});

	m_input->OnMouseReleased->Bind([this, m_input](const EUi8& button) {
		if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;

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
	});
}

void EWindow::MoveCamera()
{
	if (EGameEngine::GetGameEngine()->GetGameState() != EEGameState::GAME) return;
	
	// Get live inputs
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	glm::vec3 cameraDirection = glm::vec3(0.0f);
	if (keyState[SDL_SCANCODE_W]) cameraDirection.z += 1.0f;
	if (keyState[SDL_SCANCODE_S]) cameraDirection.z -= 1.0f;
	if (keyState[SDL_SCANCODE_A]) cameraDirection.x -= 1.0f;
	if (keyState[SDL_SCANCODE_D]) cameraDirection.x += 1.0f;
	if (keyState[SDL_SCANCODE_Q]) cameraDirection.y -= 1.0f;
	if (keyState[SDL_SCANCODE_E]) cameraDirection.y += 1.0f;

	// Get camera
	if (const auto& camRef = GetGraphicsEngine()->GetCamera().lock()) {
		// Move camera
		camRef->Translate(cameraDirection, glm::vec3(m_doubleCameraSpeed ? 2.0f : 1.0f));
		camRef->Rotate(m_cameraRotation);

		// Reset movement
		m_cameraRotation = glm::vec3(0.0f);
	}
}

void EWindow::Render()
{
	// Render the graphics engine if exists
	if (m_graphicsEngine) {
		// Update viewport to match current window size
		int w, h;
		SDL_GetWindowSize(m_sdlWindow, &w, &h);
		glViewport(0, 0, w, h);

		// Update camera aspect ratio to match new size
		if (auto camera = m_graphicsEngine->GetCamera().lock()) {
			camera->SetWindowAspectRatio({ w, h });
		}

		m_graphicsEngine->Render(m_sdlWindow);
	}
		
}

SDL_Rect EWindow::GetDisplayBounds()
{
	int displayIndex = SDL_GetWindowDisplayIndex(m_sdlWindow);
	SDL_Rect displayBounds;
	SDL_GetDisplayBounds(displayIndex, &displayBounds);
	return displayBounds;
}
