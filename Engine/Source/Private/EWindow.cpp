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

	m_deltaTime = 0.0f;

	m_defaultFrameRate = 1000;
	m_frameRate = m_defaultFrameRate;

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
		// Double camera speed
		if (key == SDL_SCANCODE_LSHIFT) {
			m_doubleCameraSpeed = true;
		}
		// Set frame rate to 10
		if (key == SDL_SCANCODE_TAB) {
			m_frameRate = 10;
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
			m_frameRate = m_defaultFrameRate;
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
	if (m_graphicsEngine)
		m_graphicsEngine->Render(m_sdlWindow);
}

void EWindow::Update()
{
	// Get delta time and cap frame rate
	// Record the previous frame time
	static double lastTickTime = 0.0;
	// Record the current frame time
	double currentTickTime = (double)SDL_GetTicks64();
	// Get the delta time - how much time has passed since the last frame
	double longDelta = currentTickTime - lastTickTime;
	// Convert from milliseconds to seconds
	m_deltaTime = float(longDelta) / 1000.0f;
	// Set the last tick time
	lastTickTime = currentTickTime;

	// Caps the frame rate
	int frameDuration = 1000 / m_frameRate;

	if ((double)frameDuration > longDelta) {
		frameDuration = (int)longDelta;
	}

	// If the frame rate is greater than m_frameRate, delay the frame
	SDL_Delay((Uint32)frameDuration);
	
	// Update the graphics engine if exists
	if (m_graphicsEngine) {
		// Test if there is a camera
		if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
			if (!m_inputMode) {
				// Translate the camera based on input direction
				glm::vec3 translateSpeed = glm::vec3(m_doubleCameraSpeed ? 2 : 1) * m_deltaTime;
				camRef->Translate(m_cameraDirection, translateSpeed);

				// Rotate the camera based on input direction
				glm::vec3 rotateSpeed = glm::abs(m_cameraRotation) * m_deltaTime;
				camRef->Rotate(m_cameraRotation, rotateSpeed);
			}
		}

		m_graphicsEngine->Update(m_deltaTime);
	}
}
