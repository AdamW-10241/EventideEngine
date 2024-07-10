#pragma once
#include "EngineTypes.h"
#include "Math/ESTransform.h"

class EGraphicsEngine;
class EInput;

struct ESWindowParams {
	// Default constructor
	ESWindowParams() {
		title = "Eventide Engine Window";
		x = y = 0;
		w = 1280;
		h = 720;
		vsync = false;
		fullscreen = false;
	}

	// Settings constructor
	ESWindowParams(EString title, int x, int y,
		unsigned int w, unsigned int h) : 
		title(title), 
		x(x), y(y), 
		w(w), h(h),
		vsync(false),
		fullscreen(false)
	{}

	// Title of the window
	EString title;
	// Position of the window
	int x, y;
	// Width and height of the window
	unsigned int w, h;
	// Vsync enable
	bool vsync;
	// Fullscreen enable
	bool fullscreen;
};

struct SDL_Window;

class EWindow {
public:
	EWindow();
	~EWindow();

	// Create the window with parameters
	bool CreateWindow(const ESWindowParams& params);

	// Close the window
	void CloseWindow() { m_shouldClose = true; }

	// Check if the window has been set to closed
	bool IsPendingClose() const { return m_shouldClose; }

	// Listen for input
	void RegisterInput(const TShared<EInput>& m_input);

	// Render the graphics engine to the window
	void Render();

private:
	// A ref to the window in SDL
	SDL_Window* m_sdlWindow;

	// Window parameters
	ESWindowParams m_params;

	// Determine if the window should close
	bool m_shouldClose;

	// Store the graphics engine
	TUnique<EGraphicsEngine> m_graphicsEngine;

	// Direction to move the camera
	glm::vec3 m_cameraDirection;

	// Amount to rotate the camera
	glm::vec3 m_cameraRotation;

	// Can zoom
	bool m_canZoom;

	// Is the user in input mode
	bool m_inputMode;

	// Double camera speed
	bool m_doubleCameraSpeed;
};